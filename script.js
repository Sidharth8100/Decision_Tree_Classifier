function uploadFile() {
    const fileName = document.getElementById('fileName').value;
    
    fetch('/upload', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ fileName })
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById('treeOutput').innerText = data.output;
    })
    .catch(error => console.error('Error:', error));
}

function predict() {
    const testFileName = document.getElementById('testFileName').value;

    fetch('/predict', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ testFileName })
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById('predictionOutput').innerText = data.result;
    })
    .catch(error => console.error('Error:', error));
}


