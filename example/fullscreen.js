var window = require('../').createWindow({
  width: 800,
  height: 600
});
var ctx = window.getContext('2d');


window.addEventListener('keydown', function(ev) {
  console.log(ev.keyCode);
  // 'f'
  if (ev.keyCode == 70) {
    if (window.fullScreen) {
      window.cancelFullScreen();
    } else {
      window.requestFullScreen();
    }
  }
});

window.requestAnimationFrame(function tick() {
  window.requestAnimationFrame(tick);

  ctx.fillStyle = "red"
  ctx.fillRect(10, 10, window.innerWidth-20, window.innerHeight-20);
  ctx.fillStyle = "green"
  ctx.fillRect(30, 30, 50, 50);
});
