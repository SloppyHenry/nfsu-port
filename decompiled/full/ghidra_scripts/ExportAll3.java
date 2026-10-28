// Export wie ExportAll2 (ohne Neuanalyse; das dauerhafte Projekt ist analysiert), zusätzlich
// callgraph.tsv, strrefs.tsv, datarefs.tsv, consts.tsv (WP-01). Argument: Ausgabeordner.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class ExportAll3 extends GhidraScript {
  PrintWriter pw(String out, String name) throws IOException {
    return new PrintWriter(new BufferedWriter(new OutputStreamWriter(new FileOutputStream(out + "/" + name), "UTF-8")));
  }

  static String clean(String s) {
    return s.replace("\\", "\\\\").replace("\t", "\\t").replace("\n", "\\n").replace("\r", "\\r");
  }

  @Override
  public void run() throws Exception {
    String out = getScriptArgs().length > 0 ? getScriptArgs()[0] : "/tmp/out3";
    new File(out).mkdirs();
    DecompInterface di = new DecompInterface();
    di.setOptions(new DecompileOptions());
    di.openProgram(currentProgram);
    Listing listing = currentProgram.getListing();
    FunctionManager fm = currentProgram.getFunctionManager();
    ReferenceManager rm = currentProgram.getReferenceManager();

    PrintWriter code = pw(out, "all_functions.c"), tsv = pw(out, "functions.tsv"), fail = pw(out, "failed.txt");
    PrintWriter cg = pw(out, "callgraph.tsv"), sr = pw(out, "strrefs.tsv"), dr = pw(out, "datarefs.tsv"), cs = pw(out, "consts.tsv");
    int n = 0, bad = 0;
    FunctionIterator it = fm.getFunctions(true);
    while (it.hasNext() && !monitor.isCancelled()) {
      Function f = it.next();
      if (f.isExternal()) continue;
      String addr = f.getEntryPoint().toString();
      String fname = f.getName(true);
      tsv.println(addr + "\t" + fname + "\t" + f.getBody().getNumAddresses() + "\t" + f.isThunk() + "\t"
          + f.getCallingConventionName() + "\t" + f.getPrototypeString(false, false));

      Set<Long> seenConst = new HashSet<>();
      Set<String> seenData = new HashSet<>();
      for (Instruction ins : listing.getInstructions(f.getBody(), true)) {
        for (Reference ref : ins.getReferencesFrom()) {
          RefType t = ref.getReferenceType();
          Address to = ref.getToAddress();
          if (t.isCall()) {
            Function callee = fm.getFunctionAt(to);
            if (callee == null && to.isExternalAddress()) {
              Symbol s = currentProgram.getSymbolTable().getPrimarySymbol(to);
              cg.println(addr + "\t" + (s != null ? "EXTERNAL:" + s.getName(true) : to) + "\t" + ins.getAddress());
            } else if (callee == null) {
              cg.println(addr + "\t" + to + "\t" + ins.getAddress());
            } else {
              cg.println(addr + "\t" + callee.getEntryPoint() + "\t" + ins.getAddress());
            }
            continue;
          }
          if (!to.isMemoryAddress() || t.isFlow()) continue;
          Data d = listing.getDataContaining(to);
          if (d != null && d.hasStringValue()) {
            if (seenData.add("s" + d.getAddress())) {
              Object v = d.getValue();
              sr.println(addr + "\t" + d.getAddress() + "\t" + clean(String.valueOf(v)));
            }
            continue;
          }
          if (fm.getFunctionContaining(to) != null) continue;
          MemoryBlock b = currentProgram.getMemory().getBlock(to);
          if (b == null || b.isExecute()) continue;
          String rw = t.isWrite() ? (t.isRead() ? "rw" : "w") : (t.isRead() ? "r" : "a");
          if (seenData.add(to + rw)) dr.println(addr + "\t" + to + "\t" + rw);
        }
        for (int op = 0; op < ins.getNumOperands(); op++) {
          for (Object o : ins.getOpObjects(op)) {
            if (o instanceof Scalar) {
              long v = ((Scalar) o).getUnsignedValue() & 0xffffffffL;
              if (v >= 0x10000 && seenConst.add(v)) cs.println(addr + "\t" + String.format("0x%08x", v) + "\t" + ins.getAddress());
            }
          }
        }
      }

      DecompileResults r = di.decompileFunction(f, 120, monitor);
      n++;
      if (r != null && r.decompileCompleted() && r.getDecompiledFunction() != null) {
        code.println("/* ==== " + addr + " " + fname + " ==== */");
        code.println(r.getDecompiledFunction().getC());
      } else {
        bad++;
        fail.println(addr + "\t" + fname + "\t" + (r == null ? "null" : r.getErrorMessage()));
      }
      if (n % 500 == 0) { println("done " + n); code.flush(); }
    }
    for (PrintWriter p : new PrintWriter[] {code, tsv, fail, cg, sr, dr, cs}) p.close();

    PrintWriter dat = pw(out, "data.tsv");
    SymbolIterator si = currentProgram.getSymbolTable().getAllSymbols(true);
    while (si.hasNext()) {
      Symbol s = si.next();
      if (s.getSymbolType() == SymbolType.LABEL) {
        Data d = listing.getDataAt(s.getAddress());
        dat.println(s.getAddress() + "\t" + s.getName() + "\t" + (d != null ? d.getDataType().getName() + "\t" + d.getLength() : "-\t0"));
      }
    }
    dat.close();
    println("EXPORT COMPLETE funcs=" + n + " failed=" + bad);
  }
}
