var hsv = require('hsv');

var boosh = require('../');

var green_hsv = hsv.rgbToHsv(0, 0, 0);
var red_hsv = hsv.rgbToHsv(255, 0, 0);
var window = boosh.createWindow();

var ctx = window.getContext('2d');
var split = require('split');
var sp = new (require('serialport').SerialPort)('/dev/tty.usbmodemfd121');

var sub = [[0,0,0,0], [0,0,0,0]];

sp.on('open' , function() {
  sp.pipe(split()).on('data', function(chunk) {
    if (!chunk) return;

    var sum = 0;
    var parts = chunk.split(':');
    var button = parseInt(parts[0], 10);

    parts[1].split(',').forEach(function(c, i) {
        sub[button][i] = (sub[button][i]*2 + parseInt(c, 10)) / 3;
        sum += sub[button][i];
    });

    sum = Math.floor(sum);

    sum > 0 && console.log(sum, button, chunk);

    var x = 100, y= 100, space = (button === 0) ? 200 : 0;


    sub[button].forEach(function(part, i) {

      part = parseInt(part, 10);
      if (isNaN(part)) {
        return;
      }

      var color = hsv.transition3(part, 900, red_hsv, green_hsv);

      ctx.fillStyle = 'rgba(' + hsv.hsvToRgb(color).map(function(i) {
        return Math.floor(i);
      }).join(',') + ', 1)';

      var width = 100;
      switch (i) {
          case 0:
              ctx.fillRect(0+space, 0, width, width);
              x-=50*(part/1024);
              y-=50*(part/1024);
          break;

          case 1:
              ctx.fillRect(100+space, 0, width, width);
              x+=50*(part/1024);
              y-=50*(part/1024);
          break;

          case 2:
              ctx.fillRect(0+space, 100, width, width);
              y+=50*(part/1024);
              x-=50*(part/1024);
          break;

          case 3:
              ctx.fillRect(100+space, 100, width, width);
              x+=50*(part/1024);
              y+=50*(part/1024);

          break;
        }
      });

      ctx.save();

      ctx.fillStyle = "#fff";
        var w = 20-(20*(sum/4096));
        ctx.fillRect((x-(w/2))+space, y -(w/2), w, w);
      ctx.restore();
  });

  window.requestAnimationFrame(function tick() {
    window.requestAnimationFrame(tick);
  });
});
