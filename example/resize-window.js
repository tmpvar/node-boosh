var window = require('../').createWindow();

var dir = 1;
window.requestAnimationFrame(function tick() {
  window.requestAnimationFrame(tick);

  window.resizeBy(dir, dir);

  if (window.outerWidth > 800) {
    dir = -1;
  } else if (window.outerWidth < 400) {
    dir = 1;
  }
});
