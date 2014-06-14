var window = require('../').createWindow();
var ctx = window.getContext('2d');


var dir = 1;
window.requestAnimationFrame(function tick() {
  window.requestAnimationFrame(tick);
  
  window.resizeBy(dir, dir);

  if (window.outerWidth > 800) {
    dir = -1;
  } else if (window.outerWidth < 400) {
    dir = 1;
  }

  ctx.fillStyle = "red"
  ctx.fillRect(10, 10, 50, 50);
  ctx.fillStyle = "green"
  ctx.fillRect(30, 30, 50, 50);
});
