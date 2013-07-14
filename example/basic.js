var window = require('../').createWindow();
var ctx = window.getContext('2d');

window.requestAnimationFrame(function tick() {
  window.requestAnimationFrame(tick);

  ctx.fillStyle = "red"
  ctx.fillRect(10, 10, 50, 50);
  ctx.fillStyle = "green"
  ctx.fillRect(30, 30, 50, 50);
});
