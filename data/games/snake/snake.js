let blockSize = 25;
let total_row = 17; // Número total de filas
let total_col = 17; // Número total de columnas
let board;
let context;

let snakeX = blockSize * 5;
let snakeY = blockSize * 5;

let speedX = 0;  // Velocidad de la serpiente en el eje X
let speedY = 0;  // Velocidad de la serpiente en el eje Y

let snakeBody = [];

let foodX;
let foodY;

let gameOver = false;

// Establecer conexión WebSocket
const ws = new WebSocket(`ws://${window.location.hostname}/ws`);

ws.onopen = () => {
  console.log('Conectado al servidor WebSocket');
};

ws.onmessage = (event) => {
  const message = event.data;
  if (message.startsWith('MOVE_')) {
    const direction = message.split('_')[1];
    changeDirection(direction);
  }
};

window.onload = function () {
    // Configurar el tablero
    board = document.getElementById("board");
    board.height = total_row * blockSize;
    board.width = total_col * blockSize;
    context = board.getContext("2d");

    placeFood();
    document.addEventListener("keyup", (e) => {
        const direction = e.key.replace('Arrow', '').toUpperCase();
        changeDirection(direction);
    });
    setInterval(update, 1000 / 10);
}

function update() {
    if (gameOver) {
        return;
    }

    // Dibujar el fondo del juego
    context.fillStyle = "green";
    context.fillRect(0, 0, board.width, board.height);

    // Dibujar la comida
    context.fillStyle = "yellow";
    context.fillRect(foodX, foodY, blockSize, blockSize);

    // Verificar si la serpiente ha comido la comida
    if (snakeX === foodX && snakeY === foodY) {
        snakeBody.push([foodX, foodY]);
        placeFood();
    }

    // Actualizar el cuerpo de la serpiente
    for (let i = snakeBody.length - 1; i > 0; i--) {
        snakeBody[i] = snakeBody[i - 1];
    }
    if (snakeBody.length) {
        snakeBody[0] = [snakeX, snakeY];
    }

    // Dibujar la serpiente
    context.fillStyle = "white";
    snakeX += speedX * blockSize;
    snakeY += speedY * blockSize;
    context.fillRect(snakeX, snakeY, blockSize, blockSize);
    for (let i = 0; i < snakeBody.length; i++) {
        context.fillRect(snakeBody[i][0], snakeBody[i][1], blockSize, blockSize);
    }

    // Verificar colisiones
    if (snakeX < 0 || snakeX >= total_col * blockSize || snakeY < 0 || snakeY >= total_row * blockSize) {
        gameOver = true;
        alert("Game Over");
    }

    for (let i = 0; i < snakeBody.length; i++) {
        if (snakeX === snakeBody[i][0] && snakeY === snakeBody[i][1]) {
            gameOver = true;
            alert("Game Over");
        }
    }
}

function changeDirection(direction) {
    if (direction === 'UP' && speedY !== 1) {
        speedX = 0;
        speedY = -1;
    } else if (direction === 'DOWN' && speedY !== -1) {
        speedX = 0;
        speedY = 1;
    } else if (direction === 'LEFT' && speedX !== 1) {
        speedX = -1;
        speedY = 0;
    } else if (direction === 'RIGHT' && speedX !== -1) {
        speedX = 1;
        speedY = 0;
    }
}

function placeFood() {
    foodX = Math.floor(Math.random() * total_col) * blockSize;
    foodY = Math.floor(Math.random() * total_row) * blockSize;
}
