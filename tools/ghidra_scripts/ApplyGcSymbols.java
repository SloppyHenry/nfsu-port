// Spielt Typen und Funktionssymbole der GC-Speed.elf in ein Ghidra-Programm ein (WP-07).
//   Typen:      refs/gc/gc_types_c.h      (aus tools/gc_types.py)
//   Funktionen: symbols/gc_functions.csv  (aus tools/gc_symbols.py)
// Setzt außerdem r13/r2 auf _SDA_BASE_/_SDA2_BASE_ (PowerPC-EABI-Small-Data).
// Aufruf als -preScript mit dem Projektwurzelpfad als Argument.
// @category NFSU
import ghidra.app.script.GhidraScript;
import ghidra.app.util.NamespaceUtils;
import ghidra.app.util.cparser.C.CParser;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.math.BigInteger;
import java.util.*;

public class ApplyGcSymbols extends GhidraScript {
  private final Map<String, DataType> types = new HashMap<>();
  private DataTypeManager dtm;

  @Override
  public void run() throws Exception {
    String root = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
    dtm = currentProgram.getDataTypeManager();
    setAnalysisOption(currentProgram, "Demangler GNU", "false");

    // 1. Typen
    CParser parser = new CParser(dtm, true, null);
    try (InputStream in = new FileInputStream(root + "/refs/gc/gc_types_c.h")) {
      parser.parse(in);
    }
    Iterator<DataType> it = dtm.getAllDataTypes();
    while (it.hasNext()) {
      DataType dt = it.next();
      types.putIfAbsent(dt.getName(), dt);
    }
    println("Typen im Programm: " + types.size());

    // 2. Small-Data-Basisregister
    setBase("r13", "_SDA_BASE_");
    setBase("r2", "_SDA2_BASE_");

    // 3. Funktionen
    int n = 0, sigs = 0, bad = 0;
    try (BufferedReader r = new BufferedReader(new FileReader(root + "/symbols/gc_functions.csv"))) {
      List<String> head = parseCsv(r.readLine());
      String line;
      while ((line = r.readLine()) != null) {
        List<String> c = parseCsv(line);
        Map<String, String> row = new HashMap<>();
        for (int i = 0; i < head.size() && i < c.size(); i++) row.put(head.get(i), c.get(i));
        try {
          if (apply(row)) sigs++;
          n++;
        } catch (Exception ex) {
          bad++;
          if (bad < 30) println("Fehler " + row.get("gc_addr") + " " + row.get("mangled") + ": " + ex.getMessage());
        }
      }
    }
    println("Funktionen benannt: " + n + ", mit Signatur: " + sigs + ", Fehler: " + bad);
  }

  private void setBase(String reg, String sym) throws Exception {
    Symbol s = getSymbols(sym, null).isEmpty() ? null : getSymbols(sym, null).get(0);
    if (s == null) { println("fehlt: " + sym); return; }
    Register r = currentProgram.getRegister(reg);
    BigInteger v = BigInteger.valueOf(s.getAddress().getOffset());
    for (MemoryBlockRange b : blocks()) {
      currentProgram.getProgramContext().setValue(r, b.start, b.end, v);
    }
    println(reg + " = " + s.getAddress());
  }

  private static class MemoryBlockRange { Address start, end; }

  private List<MemoryBlockRange> blocks() {
    List<MemoryBlockRange> out = new ArrayList<>();
    for (var b : currentProgram.getMemory().getBlocks()) {
      if (b.isExecute()) {
        MemoryBlockRange m = new MemoryBlockRange();
        m.start = b.getStart();
        m.end = b.getEnd();
        out.add(m);
      }
    }
    return out;
  }

  private boolean apply(Map<String, String> row) throws Exception {
    Address a = toAddr(row.get("gc_addr"));
    Function f = getFunctionAt(a);
    if (f == null) {
      disassemble(a);
      f = createFunction(a, null);
    }
    if (f == null) throw new Exception("keine Funktion");
    String klass = row.get("klasse");
    String meth = row.get("methode").replaceAll("\\s+", "_");
    Namespace ns = currentProgram.getGlobalNamespace();
    if (!klass.isEmpty()) {
      ns = NamespaceUtils.createNamespaceHierarchy(nsPath(klass), null, currentProgram, SourceType.IMPORTED);
      if (!(ns instanceof GhidraClass)) ns = NamespaceUtils.convertNamespaceToClass(ns);
    }
    String mangled = row.get("mangled");
    if (!f.getName().equals(meth) || !klass.isEmpty()) {
      try {
        f.setName(meth, SourceType.IMPORTED);
      } catch (ghidra.util.exception.DuplicateNameException ex) {
        // gleichnamiges ELF-Symbol existiert bereits (lokale C-Funktionen)
      }
    }
    if (!klass.isEmpty()) f.setParentNamespace(ns);
    if (!mangled.equals(meth)) {
      currentProgram.getSymbolTable().createLabel(a, mangled, SourceType.IMPORTED);
    }
    if (!row.get("demangled").equals(mangled)) {
      setPlateComment(a, row.get("demangled") + "\n" + row.get("quelldatei")
          + (row.get("zeilen").isEmpty() ? "" : ":" + row.get("zeilen")));
    }
    // Signatur nur für C++-Namen (Argumente bekannt) oder DWARF-Funktionen
    String args = row.get("argumente");
    String dwarf = row.get("parameter");
    boolean hasArgs = !row.get("demangled").equals(mangled) || !row.get("signatur_quelle").isEmpty();
    if (!hasArgs) return false;
    List<String> argTypes = new ArrayList<>();
    List<String> argNames = new ArrayList<>();
    if (!dwarf.isEmpty() && row.get("signatur_quelle").equals("dwarf-bereich")) {
      for (String p : dwarf.split("; ")) {
        String t = p.trim();
        int k = lastIdent(t);
        argTypes.add(k > 0 ? t.substring(0, k).trim() : t);
        argNames.add(k > 0 ? t.substring(k).trim() : null);
      }
    } else if (!args.isEmpty()) {
      for (String t : args.split("; ")) { argTypes.add(t.trim()); argNames.add(null); }
    }
    List<Variable> params = new ArrayList<>();
    boolean member = !klass.isEmpty() && !row.get("statisch").equals("ja");
    if (member) {
      String cls = klass.contains("::") ? klass.substring(klass.lastIndexOf("::") + 2) : klass;
      DataType ct = lookup(cls, klass);
      params.add(new ParameterImpl("this", new PointerDataType(ct == null ? DataType.VOID : ct, dtm), currentProgram));
    }
    boolean varargs = false;
    for (int i = 0; i < argTypes.size(); i++) {
      String t = argTypes.get(i);
      if (t.equals("...")) { varargs = true; continue; }
      String nm = argNames.get(i) != null && !argNames.get(i).isEmpty() ? argNames.get(i) : "p" + (i + 1);
      params.add(new ParameterImpl(uniq(nm, params), resolve(t), currentProgram));
    }
    if (meth.startsWith("~") && row.get("parameter").isEmpty()) {
      params.add(new ParameterImpl("__in_chrg", IntegerDataType.dataType, currentProgram));
    }
    Variable ret = f.getReturn();
    String rt = row.get("rueckgabe");
    if (!rt.isEmpty()) ret = new ReturnParameterImpl(resolve(rt), currentProgram);
    f.updateFunction(null, (ReturnParameterImpl) (ret instanceof ReturnParameterImpl ? ret : null), params,
        Function.FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS, true, SourceType.IMPORTED);
    if (varargs) f.setVarArgs(true);
    return true;
  }

  private static String uniq(String nm, List<Variable> ps) {
    for (Variable v : ps) if (v.getName().equals(nm)) return nm + "_" + ps.size();
    return nm;
  }

  private static int lastIdent(String t) {
    int k = t.length();
    while (k > 0 && (Character.isLetterOrDigit(t.charAt(k - 1)) || t.charAt(k - 1) == '_')) k--;
    if (k == 0 || k == t.length()) return -1;
    char c = t.charAt(k - 1);
    return (c == ' ' || c == '*' || c == '&') ? k : -1;
  }

  private static String nsPath(String klass) {
    // '::' innerhalb von Template-Argumenten nicht als Trenner werten
    StringBuilder sb = new StringBuilder();
    int depth = 0;
    for (int i = 0; i < klass.length(); i++) {
      char ch = klass.charAt(i);
      if (ch == '<') depth++;
      if (ch == '>') depth--;
      if (depth > 0 && ch == ':') { sb.append('_'); continue; }
      if (depth > 0 && (ch == ' ' || ch == ',' || ch == '*' || ch == '&')) { sb.append('_'); continue; }
      sb.append(ch);
    }
    return sb.toString();
  }

  private DataType lookup(String last, String full) {
    String a = last.replaceAll("[^A-Za-z0-9_]", "_");
    DataType dt = types.get(a);
    if (dt == null) dt = types.get(full.replaceAll("[^A-Za-z0-9_]", "_").replaceAll("__+", "_"));
    if (dt == null) dt = types.get(full.replace("::", "_").replaceAll("[^A-Za-z0-9_]", "_"));
    return dt;
  }

  private DataType resolve(String t) {
    t = t.trim();
    if (t.contains("(")) return new PointerDataType(DataType.VOID, dtm);  // Funktionszeiger
    int ptr = 0;
    while (t.endsWith("*") || t.endsWith("&") || t.endsWith(" const")) {
      if (t.endsWith(" const")) { t = t.substring(0, t.length() - 6).trim(); continue; }
      ptr++;
      t = t.substring(0, t.length() - 1).trim();
    }
    if (t.matches(".*\\[\\d+\\]$")) { ptr++; t = t.replaceAll("\\[\\d+\\]$", "").trim(); }
    t = t.replaceAll("\\b(const|volatile|struct|class|union|enum)\\b", "").replaceAll("\\s+", " ").trim();
    DataType base = builtin(t);
    if (base == null) {
      String last = t.contains("::") ? t.substring(t.lastIndexOf("::") + 2) : t;
      base = lookup(last, t);
    }
    if (base == null) base = ptr > 0 ? DataType.VOID : Undefined4DataType.dataType;
    for (int i = 0; i < ptr; i++) base = new PointerDataType(base, dtm);
    return base;
  }

  private static DataType builtin(String t) {
    switch (t) {
      case "void": return DataType.VOID;
      case "bool": return BooleanDataType.dataType;
      case "char": return CharDataType.dataType;
      case "signed char": return SignedCharDataType.dataType;
      case "unsigned char": return UnsignedCharDataType.dataType;
      case "short": case "signed short": return ShortDataType.dataType;
      case "unsigned short": return UnsignedShortDataType.dataType;
      case "int": case "signed int": case "signed": return IntegerDataType.dataType;
      case "unsigned int": case "unsigned": return UnsignedIntegerDataType.dataType;
      case "long": case "signed long": return IntegerDataType.dataType;        // 32 Bit
      case "unsigned long": return UnsignedIntegerDataType.dataType;
      case "long long": return LongLongDataType.dataType;
      case "unsigned long long": return UnsignedLongLongDataType.dataType;
      case "float": return FloatDataType.dataType;
      case "double": case "long double": return DoubleDataType.dataType;
      case "wchar_t": return WideCharDataType.dataType;
      default: return null;
    }
  }

  private static List<String> parseCsv(String line) {
    List<String> out = new ArrayList<>();
    StringBuilder cur = new StringBuilder();
    boolean q = false;
    for (int i = 0; i < line.length(); i++) {
      char ch = line.charAt(i);
      if (q) {
        if (ch == '"') {
          if (i + 1 < line.length() && line.charAt(i + 1) == '"') { cur.append('"'); i++; }
          else q = false;
        } else cur.append(ch);
      } else if (ch == '"') q = true;
      else if (ch == ',') { out.add(cur.toString()); cur.setLength(0); }
      else cur.append(ch);
    }
    out.add(cur.toString());
    return out;
  }
}
