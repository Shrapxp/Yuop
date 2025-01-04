const { execSync } = require('child_process');

// Ensure dependencies are installed (use npm or yarn)
try {
  console.log("Installing dependencies...");
  execSync('npm install', { stdio: 'inherit' });  // or use 'yarn install'
  console.log("Dependencies installed successfully.");
} catch (error) {
  console.error("Error installing dependencies:", error);
  process.exit(1);
}

// Run the Go build and Python script
try {
  console.log("Building the Go binary...");
  execSync('go build -o raja.exe raja.go', { stdio: 'inherit' });
  console.log("Go binary built successfully.");
} catch (error) {
  console.error("Error building Go binary:", error);
  process.exit(1);
}

try {
  console.log("Running Python script...");
  execSync('python d.py', { stdio: 'inherit' });
  console.log("Python script executed successfully.");
} catch (error) {
  console.error("Error running Python script:", error);
  process.exit(1);
}
