const express = require('express');
const bodyParser = require('body-parser');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = 3000;

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static('public'));

// Route to upload and train decision tree
app.post('/upload', (req, res) => {
    const fileName = req.body.fileName;
    const command = `./tree.exe ${fileName}`; // Adjust command if path differs

    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            return res.status(500).send('Error running decision tree executable');
        }
        if (stderr) {
            console.error(`Error: ${stderr}`);
            return res.status(500).send(stderr);
        }
        
        // Assuming stdout contains tree structure in JSON or parseable format
        res.send({ output: stdout });
    });
});

// Route to predict
app.post('/predict', (req, res) => {
    const testFileName = req.body.testFileName;
    const command = `./tree.exe ${testFileName} --predict`; // Example prediction command

    exec(command, (error, stdout, stderr) => {
        if (error || stderr) {
            console.error(`Error: ${error || stderr}`);
            return res.status(500).send('Error predicting data');
        }
        
        res.send({ result: stdout });
    });
});

app.listen(PORT, () => console.log(`Server running at http://localhost:${PORT}`));
