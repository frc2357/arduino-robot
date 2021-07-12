import { readFileSync } from "fs";
import { exec } from "child_process";

const executeWithOutput = (command, callback) => {
  exec(command, (error, stdout, stderr) => {
    if (error !== null) {
      console.log(`Command failed with error ${error}: ${command}`);
      console.error(stderr);
      console.log(stdout);
    }
  });
};

const loadPublishedArduinoLib = (libString) => {
  executeWithOutput(`arduino-cli lib install -v ${libString}`);
};

const loadLocalFilePathArduinoLib = (libString) => {
  console.log("TODO");
};

const loadArduinoLib = (libString) => {
  console.log(`Loading arduino lib: ${libString}`);

  if (libString.includes("@")) {
    loadPublishedArduinoLib(libString);
  } else {
    loadLocalFilePathArduinoLib(libString);
  }
};

const configFile = process.argv.length > 2 ? process.argv[2] : "package.json";

console.log(`Reading config file: ${configFile}`);

const rawFile = readFileSync(configFile);
const configJson = JSON.parse(rawFile);

configJson.arduinoLibs.forEach(loadArduinoLib);
