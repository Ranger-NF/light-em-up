const express = require("express");
const app = express();
const port = 3000;

let score = 0;

let current_players = [];

app.get("/setup", (req, res) => {
  res.send("done");
});

app.get("/data", (req, res) => {
  score++;
  res.json({
    score1: score,
    score2: score + 2,
  });
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

app.use(express.static("public"));

function checkIfGameStart() {
  let canGameStart = true;

  if (current_players.length == 2) {
    for (let eachPlayer in current_players) {
      fetch(eachPlayer).catch((err) => {
        canGameStart = false;
        current_players.remove(eachPlayer);
      });
    }
  } else {
    canGameStart = false;
  }

  if (canGameStart) {
    setInterval(() => {
      let i = 1;

      for (let eachPlayer in current_players) {
        fetch(eachPlayer)
          .then((res) => {
            if (i == 1) {
              score1 = parseInt(res.text());
            } else if (i == 2) {
              score2 = parseInt(res.text());
            }
          })
          .catch((err) => exitGameLoop());
      }

      updateDashboard();
    }, 1000);
  }
}
