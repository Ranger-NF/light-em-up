const express = require("express");
const app = express();
const port = 3000;
const path = require("path");

var os = require("os");

var networkInterfaces = os.networkInterfaces();

console.log(networkInterfaces);

app.use(express.urlencoded());
app.use(express.json());

let scores = {
  player1: 0,
  player2: 0,
};

let playerUrls = [];

let currentLoop;

app.post("/setup", (req, res) => {
  let { url } = req.body;
  res.send("done");

  playerUrls.push(url);
  console.log(playerUrls);

  if (playerUrls.length == 2) {
    currentLoop = setInterval(collectScore, 1000);
  }
});

app.get("/setup", (req, res) => {
  const options = {
    root: path.join(__dirname),
  };
  res.sendFile("form.html", options);
});

app.get("/data", (req, res) => {
  res.json({
    score1: scores.player1,
    score2: scores.player1,
  });
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

app.use(express.static("public"));

function collectScore() {
  let n = 1;
  for (let eachUrl of playerUrls) {
    console.log(eachUrl);
    fetch(eachUrl)
      .then(async (res) => {
        let collectedScore = await res.text();
        console.log(collectedScore);
        if (n == 1) {
          scores.player1 = collectedScore;
        } else {
          scores.player2 = collectedScore;
        }
      })
      .catch((err) => console.log("Error in", n, err));
  }
}
