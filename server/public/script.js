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
    ".scores .player-score:first-child .score-value"
);
const player2Score = document.querySelector(
    ".scores .player-score:last-child .score-value"
);
const player1Label = document.querySelector(
    ".scores .player-score:first-child .player-label"
);
const player2Label = document.querySelector(
    ".scores .player-score:last-child .player-label"
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

    // Convert probability to rotation angle (180 degrees arc)
    // 0% probability = -90 degrees (far left)
    // 50% probability = 0 degrees (center)
    // 100% probability = 90 degrees (far right)
    const angle = -90 + (probability * 1.8); // Maps 0-100 to -90-90 degrees
    probabilityIndicator.style.transition = "transform 0.8s cubic-bezier(0.4, 0, 0.2, 1)";
    probabilityIndicator.style.transform = `rotate(${angle}deg)`;

    setTimeout(() => {
        isAnimating = false;
    }, 800);
}

// Celebrates winner with confetti and sound
function celebrateWinner(winnerLabel, winnerScore, loserScore) {
    winSound.play();
    winnerLabel.classList.add("winner");
    
    // Hide the loser's score card
    const loserCard = winnerScore === player1Score ? 
        player2Score.parentElement : 
        player1Score.parentElement;
    loserCard.classList.add("loser");

    // Multiple confetti bursts
    const duration = 3000;
    const animationEnd = Date.now() + duration;
    const defaults = { startVelocity: 30, spread: 360, ticks: 60, zIndex: 0 };

    function randomInRange(min, max) {
        return Math.random() * (max - min) + min;
    }

    const interval = setInterval(function() {
        const timeLeft = animationEnd - Date.now();

        if (timeLeft <= 0) {
            return clearInterval(interval);
        }

        const particleCount = 50 * (timeLeft / duration);

        // Create confetti from random positions
        confetti({
            ...defaults,
            particleCount,
            origin: { x: randomInRange(0.1, 0.3), y: Math.random() - 0.2 }
        });
        confetti({
            ...defaults,
            particleCount,
            origin: { x: randomInRange(0.7, 0.9), y: Math.random() - 0.2 }
        });
    }, 250);

    // Big burst in the center
    confetti({
        particleCount: 150,
        spread: 100,
        origin: { y: 0.6 }
    });
}

// Checks if either player has won and handles game end
function checkWinner() {
    if (score1 >= WINNING_SCORE) {
        celebrateWinner(player1Label, player1Score, player2Score);
        player1Score.style.transform = "scale(1.2)";
        player1Score.style.transition = "transform 0.2s ease-out";

        player2Score.style.transform = "scale(0.8)";
        player2Score.style.transition = "transform 0.2s ease-out";
    } else if (score2 >= WINNING_SCORE) {
        celebrateWinner(player2Label, player2Score, player1Score);
        player2Score.style.transform = "scale(1.2)";
        player2Score.style.transition = "transform 0.2s ease-out";

        player1Score.style.transform = "scale(0.8)";
        player1Score.style.transition = "transform 0.2s ease-out";
    }

    setTimeout(() => {
        player1Score.style.transform = "scale(1)";
        player2Score.style.transform = "scale(1)";
    }, 200);
}

// Updates game UI with smooth animations
function updateDashboard() {
    const probability = calculateWinProbability(score1, score2);
    updateProbabilityIndicator(probability);

    // Check if game has been reset (scores back to zero)
    if (score1 === 0 && score2 === 0) {
        // Remove winner/loser classes and restore visibility
        document.querySelectorAll('.player-score').forEach(card => {
            card.classList.remove('loser');
            card.style.opacity = '';
            card.style.transform = '';
        });
        document.querySelectorAll('.player-label').forEach(label => {
            label.classList.remove('winner');
        });
    }

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
    player1Label.textContent = player1Name;
    player2Label.textContent = player2Name;

    setTimeout(() => {
        player1Score.style.transform = "scale(1)";
        player2Score.style.transition = "transform 0.2s ease-out";
        player2Score.style.transform = "scale(1)";
        player2Score.style.transition = "transform 0.2s ease-out";
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
                console.log("update");
                updateDashboard();
            }
        })
        .catch((err) => console.log(err));
}, 1000);
