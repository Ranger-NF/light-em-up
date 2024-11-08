const express = require("express");
const app = express();
const port = 3000;
const path = require("path");

var os = require("os");

var networkInterfaces = os.networkInterfaces();

console.log(networkInterfaces);

app.use(express.urlencoded());
app.use(express.json());

let names = {
  player1: "Player 1",
  player2: "Player 2",
};

let scores = {
  player1: 0,
  player2: 0,
};

let playerUrls = [];

let currentLoop;

app.post("/setup", (req, res) => {
  let { url } = req.body;
  let { playerName } = req.body;
  res.send("done");

  if (url && !playerUrls.includes(url)) {
    playerUrls.push(url);
    console.log(playerUrls);

    if (playerUrls.length == 2) {
      currentLoop = setInterval(collectScore, 1000);
    }
  }

  if (playerName && url) {
    const playerByIndex = playerUrls.findIndex((value) => value == url);

    if (playerByIndex == 0) {
      names.player1 = playerName;
    } else {
      names.player2 = playerName;
    }
    console.log(names);
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
    score2: scores.player2,
    name1: names.player1,
    name2: names.player2,
  });
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

app.use(express.static("public"));

function collectScore() {
  let n = 1;
  for (let eachUrl of playerUrls) {
    fetch(eachUrl)
      .then(async (res) => {
        let collectedScore = await res.text();
        console.log(collectedScore, n);

        if (playerUrls.findIndex((value) => value == eachUrl) == 0) {
          scores.player1 = parseInt(collectedScore);
        } else {
          scores.player2 = parseInt(collectedScore);
        }
      })
      .catch((err) => console.log("Error in", n, err));

    n += 1;
    console.log(scores);
  }
}
