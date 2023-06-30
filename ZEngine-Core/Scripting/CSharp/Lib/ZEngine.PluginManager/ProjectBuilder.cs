using Microsoft.CodeAnalysis.MSBuild;
using Microsoft.Build.Locator;

namespace ZEngine.Core
{
  public static class ProjectBuilder
  {
    public static bool Build(string projectPath, string dllOutputPath)
    {
      Console.WriteLine("Building project...");

      if (!MSBuildLocator.IsRegistered)
      {
        var locatorResults = MSBuildLocator.RegisterDefaults();
        Console.WriteLine($"Located MSBuild at path: {locatorResults.MSBuildPath}");
      }

      var workspace = MSBuildWorkspace.Create();
      var projectLoadTask = workspace.OpenProjectAsync(projectPath);
      projectLoadTask.Wait();

      var project = projectLoadTask.Result;
      Console.WriteLine($"Loaded project: {project.Name}");

      var compilationTask = project.GetCompilationAsync();

      Console.WriteLine("Waiting for compilation to complete...");
      compilationTask.Wait();

      Console.WriteLine("Waiting is complete!");

      var compilationResult = compilationTask.Result;
      if (compilationResult == null)
      {
        Console.WriteLine("Compilation failed!");
        return false;
      }

      // Write the diagnostics to the console
      foreach (var diagnostic in compilationResult.GetDiagnostics())
      {
        Console.WriteLine(diagnostic.ToString());
      }
      Console.WriteLine($"Successfully compiled project: {project.Name}");

      // Save the assembly as a DLL to a file stream
      var assemblyStream = new MemoryStream();
      var emitResult = compilationResult.Emit(assemblyStream);

      if (emitResult.Success)
      {
        Console.WriteLine($"Successfully emitted assembly for project: {project.Name}");
      }
      else
      {
        Console.WriteLine("Failed to emit assembly!");
        return false;
      }

      try {
        // Save the assembly to a file
        using (var fileStream = new FileStream(dllOutputPath, FileMode.Create))
        {
          assemblyStream.Seek(0, SeekOrigin.Begin);
          assemblyStream.CopyTo(fileStream);
        }
      } catch (Exception e) {
        Console.WriteLine($"Failed to save assembly to file: {e.Message}");
        return false;
      }

      Console.WriteLine($"Successfully saved assembly to file: {dllOutputPath}");
      return true;
    }
  }
}
