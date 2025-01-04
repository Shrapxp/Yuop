const { execSync } = require('child_process');
const path = require('path');

// Build the Go binary
try {
  console.log("Building the Go binary...");
  execSync('go build -o raja.exe raja.go', { stdio: 'inherit' });
  console.log("Go binary built successfully.");
} catch (error) {
  console.error("Error building Go binary:", error);
  process.exit(1);
}

// Install Python dependencies
try {
  console.log("Installing Python dependencies...");
  execSync('pip install -r requirements.txt', { stdio: 'inherit' });
  console.log("Python dependencies installed successfully.");
} catch (error) {
  console.error("Error installing Python dependencies:", error);
  process.exit(1);
}

// Run Python script
try {
  console.log("Running Python script...");
  execSync('python d.py', { stdio: 'inherit' });
  console.log("Python script executed successfully.");
} catch (error) {
  console.error("Error running Python script:", error);
  process.exit(1);
}
