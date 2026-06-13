/*
=========================================================================
  Facade Pattern — Practical Example: Deployment Automation
=========================================================================

Scenario:
You are building an application deployment tool. Deploying software is a 
complex, multi-step process that involves checking out code, building it, 
running tests, transferring artifacts, and activating the new version.

The Problem:
If the client (e.g., a webhook handler, CLI tool, or scheduled job) interacts
directly with each subsystem, it becomes tightly coupled to all of them.
The client has to manage the orchestration, error handling, and exact order
of operations, leading to duplicated and brittle code.

Solution:
Introduce a Facade (DeploymentFacade). This class provides a high-level, 
simplified method `deployApplication()` that orchestrates the subsystems 
internally. The client only needs to make one call, drastically reducing 
complexity and coupling.

=========================================================================
  Component Mapping
=========================================================================

  Facade      → DeploymentFacade (the unified interface)
  Subsystems  → VersionControlSystem, BuildSystem, TestingFramework, 
                DeploymentTarget (the complex parts)
  Client      → main() (wants to deploy without knowing the details)

=========================================================================
  Class Diagram
=========================================================================

                             ┌────────────────────────┐
                             │         Client         │
                             └───────────┬────────────┘
                                         │ uses
                                         ▼
           ┌──────────────────────────────────────────────────────────┐
           │                     DeploymentFacade                     │
           ├──────────────────────────────────────────────────────────┤
           │ - vcs: VersionControlSystem                              │
           │ - buildSystem: BuildSystem                               │
           │ - testingFramework: TestingFramework                     │
           │ - deploymentTarget: DeploymentTarget                     │
           ├──────────────────────────────────────────────────────────┤
           │ + deployApplication(branch: string, server: string): bool│
           └─────┬───────────────┬───────────────┬────────────────┬───┘
                 │               │               │                │
       ┌─────────┘               │               │                └─────────┐
       ▼                         ▼               ▼                          ▼
┌───────────────┐        ┌───────────────┐ ┌──────────────────┐  ┌──────────────────┐
│VersionControl │        │  BuildSystem  │ │ TestingFramework │  │ DeploymentTarget │
│    System     │        ├───────────────┤ ├──────────────────┤  ├──────────────────┤
├───────────────┤        │+compile():bool│ │+runUnitTests()   │  │+transferArtifact(│
│+pullLatest    │        │+getArtifact() │ │+runIntegration...│  │  path, server)   │
│ Changes(branch│        └───────────────┘ └──────────────────┘  │+activateNewVer...│
└───────────────┘                                                └──────────────────┘

=========================================================================
*/

#include <iostream>
#include <thread>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: The Subsystems
=========================================================================
These are the complex, low-level modules. They do the actual heavy lifting.
Notice how many steps and methods are involved.
*/

class VersionControlSystem {
public:
  void pullLatestChanges(string branch) {
    cout << "[VCS] Pulling latest changes from '" << branch << "'..." << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "[VCS] Pull complete." << endl;
  }
};

class BuildSystem {
public:
  bool compileProject() {
    cout << "[Build] Compiling project..." << endl;
    this_thread::sleep_for(chrono::milliseconds(800));
    cout << "[Build] Build successful." << endl;
    return true;
  }

  string getArtifactPath() {
    string path = "target/myapplication-1.0.jar";
    cout << "[Build] Artifact located at " << path << endl;
    return path;
  }
};

class TestingFramework {
public:
  bool runUnitTests() {
    cout << "[Test] Running unit tests..." << endl;
    this_thread::sleep_for(chrono::milliseconds(600));
    cout << "[Test] Unit tests passed." << endl;
    return true;
  }

  bool runIntegrationTests() {
    cout << "[Test] Running integration tests..." << endl;
    this_thread::sleep_for(chrono::milliseconds(900));
    cout << "[Test] Integration tests passed." << endl;
    return true;
  }
};

class DeploymentTarget {
public:
  void transferArtifact(string artifactPath, string server) {
    cout << "[Deploy] Transferring " << artifactPath << " to " << server << "..." << endl;
    this_thread::sleep_for(chrono::milliseconds(700));
    cout << "[Deploy] Transfer complete." << endl;
  }

  void activateNewVersion(string server) {
    cout << "[Deploy] Activating new version on " << server << "..." << endl;
    this_thread::sleep_for(chrono::milliseconds(400));
    cout << "[Deploy] Now live on " << server << "!" << endl;
  }
};

/*
=========================================================================
  STEP 2: The Facade
=========================================================================
The DeploymentFacade serves as a single, unified interface. Internally, 
it holds references to the subsystems and orchestrates the complex workflow.
If any step fails, it handles the error and returns false.
*/

class DeploymentFacade {
private:
  VersionControlSystem vcs;
  BuildSystem buildSystem;
  TestingFramework testingFramework;
  DeploymentTarget deploymentTarget;

public:
  bool deployApplication(string branch, string serverAddress) {
    cout << "\n=== FACADE: Initiating Deployment for '" << branch 
         << "' to " << serverAddress << " ===" << endl;
    
    try {
      // 1. Version Control
      vcs.pullLatestChanges(branch);

      // 2. Build
      if (!buildSystem.compileProject()) {
        cerr << "FACADE ERROR: Build compilation failed." << endl;
        return false;
      }
      string artifactPath = buildSystem.getArtifactPath();

      // 3. Testing
      if (!testingFramework.runUnitTests()) {
        cerr << "FACADE ERROR: Unit tests failed." << endl;
        return false;
      }
      if (!testingFramework.runIntegrationTests()) {
        cerr << "FACADE ERROR: Integration tests failed." << endl;
        return false;
      }

      // 4. Deployment
      deploymentTarget.transferArtifact(artifactPath, serverAddress);
      deploymentTarget.activateNewVersion(serverAddress);

      cout << "=== FACADE: Deployment SUCCESSFUL! ===" << endl;
      return true;

    } catch (exception &e) {
      cerr << "FACADE ERROR: Unexpected error occurred: " << e.what() << endl;
      return false;
    }
  }
};

/*
=========================================================================
  STEP 3: Client Code
=========================================================================
Compare the complexity. The client used to orchestrate all subsystems
directly. Now, the client just talks to the Facade.
*/

void runClientWithoutFacade() {
  cout << "\n--- Scenario 1: Client WITHOUT Facade ---" << endl;
  
  string branch = "main";
  string server = "prod.server.example.com";

  // Client creates and manages all subsystems directly
  VersionControlSystem vcs;
  BuildSystem buildSystem;
  TestingFramework testFramework;
  DeploymentTarget deployTarget;

  vcs.pullLatestChanges(branch);
  
  if (buildSystem.compileProject()) {
    string artifact = buildSystem.getArtifactPath();
    
    if (testFramework.runUnitTests() && testFramework.runIntegrationTests()) {
      deployTarget.transferArtifact(artifact, server);
      deployTarget.activateNewVersion(server);
      cout << "[Client] Deployment successful!\n";
    } else {
      cerr << "[Client] Tests failed. Aborting.\n";
    }
  } else {
    cerr << "[Client] Build failed. Aborting.\n";
  }
}

void runClientWithFacade() {
  cout << "\n--- Scenario 2: Client WITH Facade ---" << endl;
  
  // Client only interacts with the unified Facade
  DeploymentFacade facade;
  facade.deployApplication("feature/new-ui", "staging.server.example.com");
}

int main() {
  runClientWithoutFacade();
  runClientWithFacade();
  
  return 0;
}