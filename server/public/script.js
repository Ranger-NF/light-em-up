// Game state variables
let score1 = 0;
let score2 = 0;
const WINNING_SCORE = 10;
let isAnimating = false;
let player1Name = "Player 1"; // Player 1 name variable
let player2Name = "Player 2"; // Player 2 name variable

// DOM elements for game UI
const probabilityIndicator = document.querySelector(".probability-indicator");
const player1Score = document.querySelector(
  ".scores .player-score:first-child .score-value",
);
const player2Score = document.querySelector(
  ".scores .player-score:last-child .score-value",
);
const player1Label = document.querySelector(
  ".scores .player-score:first-child .player-label",
);
const player2Label = document.querySelector(
  ".scores .player-score:last-child .player-label",
);
const winSound = document.getElementById("winSound");

// Calculates win probability percentage based on player scores
function calculateWinProbability(score1, score2) {
  const totalScore = score1 + score2;
  if (totalScore === 0) return 50;
  const probability = (score2 / totalScore) * 100;
  return Math.min(Math.max(probability, 0), 100);
}

// Animates probability indicator position with smooth transition
function updateProbabilityIndicator(probability) {
  if (isAnimating) return;
  isAnimating = true;

  const position = probability + "%";
  probabilityIndicator.style.transition =
    "left 0.8s cubic-bezier(0.4, 0, 0.2, 1)";
  probabilityIndicator.style.left = position;

  setTimeout(() => {
    isAnimating = false;
  }, 800);
}

// Celebrates winner with confetti and sound
function celebrateWinner(winnerLabel) {
  winSound.play();
  winnerLabel.classList.add("winner");

  confetti({
    particleCount: 100,
    spread: 70,
    origin: { y: 0.6 },
  });
}

// Checks if either player has won and handles game end
function checkWinner() {
  if (score1 >= WINNING_SCORE) {
    celebrateWinner(player1Label);
    player1Name.style.transform = "scale(1.2)";
    player1Name.style.transition = "transform 0.2s ease-out";

    player2Name.style.transform = "scale(0.8)";
    player2Name.style.transition = "transform 0.2s ease-out";
  } else if (score2 >= WINNING_SCORE) {
    celebrateWinner(player2Label);
    player2Name.style.transform = "scale(1.2)";
    player2Name.style.transition = "transform 0.2s ease-out";

    player1Name.style.transform = "scale(0.8)";
    player1Name.style.transition = "transform 0.2s ease-out";
  }

  setTimeout(() => {
    player1Name.style.transform = "scale(1)";
    player2Name.style.transform = "scale(1)";
  }, 200);
  x;
}

// Updates game UI with smooth animations
function updateDashboard() {
  const probability = calculateWinProbability(score1, score2);
  updateProbabilityIndicator(probability);

  // Animate score changes with scale effect
  const currentScore1 = parseInt(player1Score.textContent);
  const currentScore2 = parseInt(player2Score.textContent);

  if (currentScore1 !== score1) {
    player1Score.style.transform = "scale(1.2)";
    player1Score.style.transition = "transform 0.2s ease-out";
  }
  if (currentScore2 !== score2) {
    player2Score.style.transform = "scale(1.2)";
    player2Score.style.transition = "transform 0.2s ease-out";
  }

  player1Score.textContent = score1;
  player2Score.textContent = score2;
  player1Score.style.color = "#2171cc";
  player2Score.style.color = "#d62b54";

  // Update player labels with names
  player1Label.textContent = player1Name;
  player2Label.textContent = player2Name;

  setTimeout(() => {
    player1Score.style.transform = "scale(1)";
    player2Score.style.transform = "scale(1)";
  }, 200);

  checkWinner();
}

// Initialize game dashboard
updateDashboard();

setInterval(() => {
  fetch(window.location.href + "data")
    .then(async (res) => {
      let data = await res.json();

      let lastScore1 = score1;
      let lastScore2 = score2;

      let lastPlayer1 = player1Name;
      let lastPlayer2 = player2Name;

      score1 = data.score1;
      score2 = data.score2;

      player1Name = data.name1;
      player2Name = data.name2;

      if (
        score1 != lastScore1 ||
        score2 != lastScore2 ||
        lastPlayer1 != player1Name ||
        lastPlayer2 != player2Name
      ) {
        updateDashboard();
      }
    })
    .then((err) => console.log(err));
}, 1000);
