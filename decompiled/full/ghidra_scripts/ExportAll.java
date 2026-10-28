import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import java.io.*;

public class ExportAll extends GhidraScript {
  @Override
  public void run() throws Exception {
    String out = getScriptArgs().length > 0 ? getScriptArgs()[0] : "/tmp/out";
    new File(out).mkdirs();
    DecompInterface di = new DecompInterface();
    DecompileOptions opt = new DecompileOptions();
    di.setOptions(opt);
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
      tsv.println(addr + "\t" + f.getName() + "\t" + f.getBody().getNumAddresses() + "\t" + f.isThunk() + "\t" + f.getCallingConventionName() + "\t" + f.getPrototypeString(false, false));
      DecompileResults r = di.decompileFunction(f, 120, monitor);
      n++;
      if (r != null && r.decompileCompleted() && r.getDecompiledFunction() != null) {
        code.println("/* ==== " + addr + " " + f.getName() + " ==== */");
        code.println(r.getDecompiledFunction().getC());
      } else {
        bad++;
        fail.println(addr + "\t" + f.getName() + "\t" + (r == null ? "null" : r.getErrorMessage()));
      }
      if (n % 500 == 0) { println("done " + n); code.flush(); }
    }
    code.close(); tsv.close(); fail.close();
    // data symbols
    PrintWriter dat = new PrintWriter(new BufferedWriter(new FileWriter(out + "/data.tsv")));
    SymbolIterator si = currentProgram.getSymbolTable().getAllSymbols(true);
    while (si.hasNext()) {
      Symbol s = si.next();
      if (s.getSymbolType() == SymbolType.LABEL) {
        Data d = currentProgram.getListing().getDataAt(s.getAddress());
        dat.println(s.getAddress() + "\t" + s.getName() + "\t" + (d != null ? d.getDataType().getName() + "\t" + d.getLength() : "-\t0"));
      }
    }
    dat.close();
    println("EXPORT COMPLETE funcs=" + n + " failed=" + bad);
  }
}
