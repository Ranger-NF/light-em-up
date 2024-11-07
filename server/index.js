const express = require("express");

let last_id = 0;

let current_players = {};
let active_player_count = 0;

const app = express();
const port = process.env.PORT || 3000;

// Serve static files
app.get("/newgame", (req, res) => {
  let ip = req.headers["x-forwarded-for"] || req.socket.remoteAddress;

  if (active_player_count > 0 && active_player_count < 2) {
    last_id++;
    current_players[last_id] = {
      score: 0,
      mcu_id: req.body.mcu_id,
      identifier: ip,
    };
    active_player_count += 1;

    res.send(last_id);
  } else {
    current_players = {};
    active_player_count = 0;
    res.send(0);
  }
});

app.post("/score", (req, res) => {
  const mcu_id = req.body.mcu_id;
  const score = req.body.score;

  if (current_players.has(mcu_id)) {
    current_players[mcu_id].score = score;
    res.send(1);
  } else {
    res.send(0);
  }
});

app.get("/", (req, res) => {
  res.send(current_players);
});

// Set up Express server
app.listen(port, () => {
  console.log(`Server running on port ${port}`);
});

function check_score() {
  for (each_player in current_players) {
    if (each_player.score > 10) {
      return;
    }
  }
}
