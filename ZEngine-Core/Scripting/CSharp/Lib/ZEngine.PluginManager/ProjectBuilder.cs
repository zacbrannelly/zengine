using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.MSBuild;
using Microsoft.Build.Locator;

namespace ZEngine.Core
{
  public static class ProjectBuilder
  {
    private static string GetInteropReferenceDirectory()
    {
      // The editor copies ZEngine.Interop beside the executable under lib/ZEngine.Interop.
      return Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "lib", "ZEngine.Interop"));
    }

    private static string GetInteropReferencePath()
    {
      return Path.Combine(GetInteropReferenceDirectory(), "ZEngine.Interop.dll");
    }

    public static bool Build(string projectPath, string dllOutputPath)
    {
      Console.WriteLine("Building project...");

      if (!MSBuildLocator.IsRegistered)
      {
        var locatorResults = MSBuildLocator.RegisterDefaults();
        Console.WriteLine($"Located MSBuild at path: {locatorResults.MSBuildPath}");
      }

      var interopReferenceDirectory = GetInteropReferenceDirectory();
      var interopReferencePath = GetInteropReferencePath();
      Console.WriteLine($"Using interop reference directory: {interopReferenceDirectory}");
      Console.WriteLine($"Using interop reference path: {interopReferencePath}");

      var workspaceProperties = new Dictionary<string, string>
      {
        // Allow <Reference Include="ZEngine.Interop" /> to resolve without embedding
        // an absolute HintPath into every generated user project.
        ["ReferencePath"] = interopReferencePath
      };

      var workspace = MSBuildWorkspace.Create(workspaceProperties);
      var projectLoadTask = workspace.OpenProjectAsync(projectPath);
      projectLoadTask.Wait();

      var project = projectLoadTask.Result;
      if (!File.Exists(interopReferencePath))
      {
        Console.WriteLine($"Interop reference was not found at: {interopReferencePath}");
        return false;
      }

      project = project.AddMetadataReference(MetadataReference.CreateFromFile(interopReferencePath));
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
        // Make sure the output directory exists
        var outputDirectory = Path.GetDirectoryName(dllOutputPath);
        if (!Directory.Exists(outputDirectory))
        {
          Directory.CreateDirectory(outputDirectory);
        }

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
