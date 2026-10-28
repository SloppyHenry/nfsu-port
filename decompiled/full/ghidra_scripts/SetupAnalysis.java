// Einmalig nach dem Import ins dauerhafte Projekt: dieselbe Zusatzanalyse wie ExportAll2.
import ghidra.app.script.GhidraScript;

public class SetupAnalysis extends GhidraScript {
  @Override
  public void run() throws Exception {
    setAnalysisOption(currentProgram, "Decompiler Parameter ID", "true");
    analyzeAll(currentProgram);
    println("SETUP ANALYSIS DONE");
  }
}
