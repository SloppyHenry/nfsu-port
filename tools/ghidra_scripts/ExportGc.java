// Headless-Export des GC-Programms im Format von decompiled/full/out2 (WP-07).
// Unterschied zu ExportAll2: Namen mit Namensraum (Klasse::Methode), keine erneute Analyse.
// Aufruf als -postScript mit dem Ausgabeordner als Argument.
// @category NFSU
import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class ExportGc extends GhidraScript {
  @Override
  public void run() throws Exception {
    String out = getScriptArgs().length > 0 ? getScriptArgs()[0] : "/tmp/out";
    new File(out).mkdirs();
    DecompInterface di = new DecompInterface();
    di.setOptions(new DecompileOptions());
    di.openProgram(currentProgram);
    PrintWriter code = new PrintWriter(new BufferedWriter(new FileWriter(out + "/all_functions.c")));
    PrintWriter tsv = new PrintWriter(new BufferedWriter(new FileWriter(out + "/functions.tsv")));
    PrintWriter fail = new PrintWriter(new BufferedWriter(new FileWriter(out + "/failed.txt")));
    FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
    int n = 0, bad = 0;
    while (it.hasNext() && !monitor.isCancelled()) {
      Function f = it.next();
      if (f.isExternal()) continue;
      String addr = f.getEntryPoint().toString();
      String name = f.getName(true);
      tsv.println(addr + "\t" + name + "\t" + f.getBody().getNumAddresses() + "\t" + f.isThunk() + "\t"
          + f.getCallingConventionName() + "\t" + f.getPrototypeString(false, false));
      DecompileResults r = di.decompileFunction(f, 120, monitor);
      n++;
      if (r != null && r.decompileCompleted() && r.getDecompiledFunction() != null) {
        code.println("/* ==== " + addr + " " + name + " ==== */");
        code.println(r.getDecompiledFunction().getC());
      } else {
        bad++;
        fail.println(addr + "\t" + name + "\t" + (r == null ? "null" : r.getErrorMessage()));
      }
      if (n % 500 == 0) { println("done " + n); code.flush(); }
    }
    code.close(); tsv.close(); fail.close();
    PrintWriter dat = new PrintWriter(new BufferedWriter(new FileWriter(out + "/data.tsv")));
    SymbolIterator si = currentProgram.getSymbolTable().getAllSymbols(true);
    while (si.hasNext()) {
      Symbol s = si.next();
      if (s.getSymbolType() == SymbolType.LABEL) {
        Data d = currentProgram.getListing().getDataAt(s.getAddress());
        dat.println(s.getAddress() + "\t" + s.getName(true) + "\t"
            + (d != null ? d.getDataType().getName() + "\t" + d.getLength() : "-\t0"));
      }
    }
    dat.close();
    println("EXPORT DONE " + n + " functions, " + bad + " failed");
  }
}
