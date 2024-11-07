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

// Game state variables
let score1 = 0;
let score2 = 0;
const WINNING_SCORE = 10;
let isAnimating = false;
const player1Name = "Player 1"; // Player 1 name variable
const player2Name = "Player 2"; // Player 2 name variable

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

// Resets game state and animates score transitions
function restartGame() {
  // Fade out scores with animation
  player1Score.style.transition = "opacity 0.3s ease-out";
  player2Score.style.transition = "opacity 0.3s ease-out";
  player1Score.style.opacity = "0";
  player2Score.style.opacity = "0";

  setTimeout(() => {
    score1 = 0;
    score2 = 0;
    player1Label.classList.remove("winner");
    player2Label.classList.remove("winner");

    // Reset scores and fade back in
    updateDashboard();
    player1Score.style.transition = "opacity 0.3s ease-in";
    player2Score.style.transition = "opacity 0.3s ease-in";
    player1Score.style.opacity = "1";
    player2Score.style.opacity = "1";
  }, 300);
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
    setTimeout(restartGame, 2000);
  } else if (score2 >= WINNING_SCORE) {
    celebrateWinner(player2Label);
    setTimeout(restartGame, 2000);
  }
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

// Fetches latest scores from NodeMCU server
async function fetchScores() {
  try {
    const response = await fetch("http://192.168.4.1/scores");
    if (!response.ok) throw new Error("Network response was not ok");

    const data = await response.json();
    score1 = Math.min(data.score1, WINNING_SCORE);
    score2 = Math.min(data.score2, WINNING_SCORE);
    updateDashboard();
  } catch (error) {
    console.error("Error fetching scores:", error);
    // Visual feedback for error state
    probabilityIndicator.style.opacity = "0.5";
    setTimeout(() => {
      probabilityIndicator.style.opacity = "1";
    }, 300);
  }
}

// Initialize game dashboard
updateDashboard();

// Poll for score updates every 100ms
setInterval(fetchScores, 100);
