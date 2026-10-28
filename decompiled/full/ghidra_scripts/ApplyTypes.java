// Spielt symbols/types.h und symbols/com_types.h ein und wendet sie an (WP-04).
//  - this-Parameter von Klasse::Methode (thiscall/fastcall) bekommt Klasse * (wenn die Klasse in types.h steht)
//  - Vtables: Label vtbl_<Klasse> (symbols/vtables.csv, nur Klassen ohne Fragezeichen), pro Slot ein Zeiger-Eintrag
//  - COM-Globals (symbols/com_globals.csv) erhalten Zeiger auf die Schnittstelle
//  - COM-Aufrufe (symbols/com_calls.csv) erhalten einen Zeilenkommentar Schnittstelle::Methode
// Argument: Pfad zu symbols/. Läuft nach ApplySymbols.java.
import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParserUtils;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class ApplyTypes extends GhidraScript {
  static List<Map<String, String>> readCsv(File f) throws IOException {
    List<Map<String, String>> rows = new ArrayList<>();
    try (BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(f), "UTF-8"))) {
      List<String> head = ApplySymbols.parseLine(r.readLine());
      String line;
      while ((line = r.readLine()) != null) {
        if (line.isEmpty()) continue;
        List<String> v = ApplySymbols.parseLine(line);
        Map<String, String> m = new HashMap<>();
        for (int i = 0; i < head.size(); i++) m.put(head.get(i), i < v.size() ? v.get(i) : "");
        rows.add(m);
      }
    }
    return rows;
  }

  @Override
  public void run() throws Exception {
    String dir = getScriptArgs().length > 0 ? getScriptArgs()[0] : "../../symbols";
    DataTypeManager dtm = currentProgram.getDataTypeManager();
    // 1. Typen einlesen (Doppelt eingelesene Typen werden ersetzt)
    String[] hdrs = { new File(dir, "types.h").getPath(), new File(dir, "com_types.h").getPath() };
    CParserUtils.parseHeaderFiles(new DataTypeManager[0], hdrs, new String[0], new String[0], dtm, monitor);
    int ntypes = 0;
    Iterator<Structure> si = dtm.getAllStructures();
    while (si.hasNext()) { si.next(); ntypes++; }

    Map<String, DataType> byName = new HashMap<>();
    Iterator<DataType> di = dtm.getAllDataTypes();
    while (di.hasNext()) {
      DataType d = di.next();
      if (d instanceof Structure || d instanceof Union) byName.put(d.getName(), d);
    }

    // 2. this-Parameter
    FunctionManager fm = currentProgram.getFunctionManager();
    int thisTyped = 0, thisSkip = 0, errors = 0;
    Set<String> classes = new HashSet<>();
    for (Map<String, String> r : readCsv(new File(dir, "functions.csv"))) {
      String name = r.get("name");
      int k = name.lastIndexOf("::");
      if (k <= 0) continue;
      String cls = name.substring(0, k);
      if (cls.contains("::")) cls = cls.substring(cls.lastIndexOf("::") + 2);
      DataType dt = byName.get(cls);
      if (dt == null) { thisSkip++; continue; }
      Function f = fm.getFunctionAt(toAddr(r.get("addr")));
      if (f == null || f.getParameterCount() == 0) continue;
      String cc = f.getCallingConventionName();
      if (!"__thiscall".equals(cc) && !"__fastcall".equals(cc)) continue;
      Parameter p = f.getParameter(0);
      try {
        if (p.isAutoParameter()) {
          f.setCustomVariableStorage(true);   // Auto-this wird zum normalen Parameter (Speicher ECX bleibt)
          p = f.getParameter(0);
        }
        p.setDataType(new PointerDataType(dt), SourceType.USER_DEFINED);
        if (p.getName().startsWith("param_")) p.setName("this", SourceType.USER_DEFINED);
        thisTyped++;
        classes.add(cls);
      } catch (Exception e) { errors++; println("FEHLER this " + name + ": " + e.getMessage()); }
    }

    // 3. Vtables: Label + Zeigerfelder
    int vlabels = 0;
    SymbolTable st = currentProgram.getSymbolTable();
    Set<String> seen = new HashSet<>();
    Map<String, Integer> idx = new HashMap<>();
    Map<Address, Integer> len = new LinkedHashMap<>();
    Map<Address, String> vname = new LinkedHashMap<>();
    for (Map<String, String> r : readCsv(new File(dir, "vtables.csv"))) {
      Address a = toAddr(r.get("vtable_addr"));
      len.merge(a, 1, Integer::sum);
      String cls = r.get("klasse");
      if (!cls.equals("?")) vname.put(a, cls.replace("?", "").replace("/", "__").replace("::", "_").replace("@", "_"));
    }
    for (Map.Entry<Address, String> e : vname.entrySet()) {
      String base = "vtbl_" + e.getValue();
      int n = idx.merge(base, 1, Integer::sum);
      String nm = n == 1 ? base : base + "_" + n;
      try {
        Symbol s = st.createLabel(e.getKey(), nm, SourceType.USER_DEFINED);
        s.setPrimary();
        vlabels++;
      } catch (Exception ex) { errors++; println("FEHLER vtable " + nm + ": " + ex.getMessage()); }
    }

    // 4. COM-Globals
    int comGlobals = 0;
    for (Map<String, String> r : readCsv(new File(dir, "com_globals.csv"))) {
      String ort = r.get("ort");
      if (!ort.startsWith("G:")) continue;
      DataType dt = byName.get(r.get("interface"));
      if (dt == null) continue;
      Address a = toAddr(ort.substring(2));
      try {
        Listing l = currentProgram.getListing();
        l.clearCodeUnits(a, a.add(3), false);
        l.createData(a, new PointerDataType(dt));
        comGlobals++;
      } catch (Exception ex) { errors++; println("FEHLER com-global " + ort + ": " + ex.getMessage()); }
    }

    // 5. COM-Aufrufe als Kommentar
    int comCalls = 0;
    for (Map<String, String> r : readCsv(new File(dir, "com_calls.csv"))) {
      setEOLComment(toAddr(r.get("aufruf")), r.get("interface") + "::" + r.get("methode"));
      comCalls++;
    }
    println("TYPES DONE structs=" + ntypes + " this=" + thisTyped + " classes=" + classes.size()
        + " thisSkipNoType=" + thisSkip + " vtables=" + vlabels + " comGlobals=" + comGlobals
        + " comCalls=" + comCalls + " errors=" + errors);
  }
}
