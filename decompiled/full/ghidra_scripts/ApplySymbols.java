// Spielt symbols/functions.csv und symbols/data.csv ins Programm ein (WP-01).
// Argument: Pfad zu symbols/ (Standard: ../../symbols relativ zum Projektordner).
// Prototypen werden nur ab Status "typisiert" gesetzt und nur, wenn sie keine FUN_-Platzhalter enthalten.
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.app.util.parser.FunctionSignatureParser;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.FunctionDefinitionDataType;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.app.util.NamespaceUtils;
import java.io.*;
import java.util.*;

public class ApplySymbols extends GhidraScript {
  static final Set<String> TYPED = new HashSet<>(Arrays.asList(
      "typisiert", "rekonstruiert", "getestet", "ersetzt"));

  static List<String> parseLine(String line) {
    List<String> out = new ArrayList<>();
    StringBuilder sb = new StringBuilder();
    boolean q = false;
    for (int i = 0; i < line.length(); i++) {
      char c = line.charAt(i);
      if (q) {
        if (c == '"') {
          if (i + 1 < line.length() && line.charAt(i + 1) == '"') { sb.append('"'); i++; }
          else q = false;
        } else sb.append(c);
      } else if (c == '"') q = true;
      else if (c == ',') { out.add(sb.toString()); sb.setLength(0); }
      else sb.append(c);
    }
    out.add(sb.toString());
    return out;
  }

  static List<Map<String, String>> readCsv(File f) throws IOException {
    List<Map<String, String>> rows = new ArrayList<>();
    try (BufferedReader r = new BufferedReader(new InputStreamReader(new FileInputStream(f), "UTF-8"))) {
      List<String> head = parseLine(r.readLine());
      String line;
      while ((line = r.readLine()) != null) {
        if (line.isEmpty()) continue;
        List<String> v = parseLine(line);
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
    FunctionManager fm = currentProgram.getFunctionManager();
    SymbolTable st = currentProgram.getSymbolTable();
    int named = 0, protos = 0, comments = 0, missing = 0, bad = 0;
    FunctionSignatureParser parser = new FunctionSignatureParser(currentProgram.getDataTypeManager(), null);

    for (Map<String, String> r : readCsv(new File(dir, "functions.csv"))) {
      Address a = toAddr(r.get("addr"));
      Function f = fm.getFunctionAt(a);
      if (f == null) { missing++; continue; }
      String name = r.get("name");
      try {
        if (!name.isEmpty() && !name.equals(f.getName(true)) && !name.equals(f.getName())) {
          if (name.contains("::")) {
            int k = name.lastIndexOf("::");
            Namespace ns = NamespaceUtils.createNamespaceHierarchy(name.substring(0, k), null, currentProgram, SourceType.USER_DEFINED);
            f.setParentNamespace(ns);
            f.setName(name.substring(k + 2), SourceType.USER_DEFINED);
          } else {
            f.setName(name, SourceType.USER_DEFINED);
          }
          named++;
        }
        String proto = r.get("prototype");
        if (TYPED.contains(r.get("status")) && !proto.isEmpty() && !proto.contains("FUN_")) {
          FunctionDefinitionDataType sig = parser.parse(f.getSignature(), proto);
          new ApplyFunctionSignatureCmd(a, sig, SourceType.USER_DEFINED).applyTo(currentProgram, monitor);
          protos++;
        }
        String c = "[" + r.get("module") + "|" + r.get("kind") + "|" + r.get("status") + "|" + r.get("confidence") + "] " + r.get("note");
        if (!r.get("module").isEmpty() || !r.get("note").isEmpty()) { f.setComment(c.trim()); comments++; }
      } catch (Exception e) {
        bad++;
        println("FEHLER " + r.get("addr") + " " + name + ": " + e.getMessage());
      }
    }

    int dnamed = 0;
    File df = new File(dir, "data.csv");
    if (df.exists()) {
      for (Map<String, String> r : readCsv(df)) {
        Address a = toAddr(r.get("addr"));
        Symbol s = st.getPrimarySymbol(a);
        if (s != null && s.getName().equals(r.get("name"))) continue;
        Symbol ns = st.createLabel(a, r.get("name"), SourceType.USER_DEFINED);
        ns.setPrimary();
        if (!r.get("note").isEmpty()) setEOLComment(a, r.get("note"));
        dnamed++;
      }
    }
    println("APPLY DONE named=" + named + " protos=" + protos + " comments=" + comments
        + " missing=" + missing + " errors=" + bad + " data=" + dnamed);
  }
}
