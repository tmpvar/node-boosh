var window = require('../').createWindow({
  width: 800,
  height: 600,
  title: 'fullscreen demo'
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

  if (window.fullScreen) {
    ctx.fillStyle = "green"
  } else {
    ctx.fillStyle = 'rgba(255, 0, 0, 1)'
  }
  ctx.fillRect(10, 10, window.innerWidth-20, window.innerHeight-20);
});
