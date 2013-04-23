/**
* Converts an RGB color value to HSV. Conversion formula
* adapted from http://en.wikipedia.org/wiki/HSV_color_space.
* Assumes r, g, and b are contained in the set [0, 255] and
* returns h, s, and v in the set [0, 1].
*
* @param Number r The red color value
* @param Number g The green color value
* @param Number b The blue color value
* @return Array The HSV representation
*/
function rgbToHsv(r, g, b){
    r = r/255, g = g/255, b = b/255;
    var max = Math.max(r, g, b), min = Math.min(r, g, b);
    var h, s, v = max;

    var d = max - min;
    s = max == 0 ? 0 : d / max;

    if(max == min){
        h = 0; // achromatic
    }else{
        switch(max){
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }

    return [h, s, v];
}

/**
* Converts an HSV color value to RGB. Conversion formula
* adapted from http://en.wikipedia.org/wiki/HSV_color_space.
* Assumes h, s, and v are contained in the set [0, 1] and
* returns r, g, and b in the set [0, 255].
*
* @param Number h The hue
* @param Number s The saturation
* @param Number v The value
* @return Array The RGB representation
*/
/*function hsvToRgb(h, s, v){
var r, g, b;

var i = Math.floor(h * 6);
var f = h * 6 - i;
var p = v * (1 - s);
var q = v * (1 - f * s);
var t = v * (1 - (1 - f) * s);

switch(i % 6){
case 0: r = v, g = t, b = p; break;
case 1: r = q, g = v, b = p; break;
case 2: r = p, g = v, b = t; break;
case 3: r = p, g = q, b = v; break;
case 4: r = t, g = p, b = v; break;
case 5: r = v, g = p, b = q; break;
}

return [r * 255, g * 255, b * 255];
}*/


function hsvToRgb(hsv)
{
    var r, g, b;

    var h = hsv[0];
    var s = hsv[1];
    var v = hsv[2];

    var i = Math.floor(h * 6);
    var f = h * 6 - i;
    var p = v * (1 - s);
    var q = v * (1 - f * s);
    var t = v * (1 - (1 - f) * s);

    switch(i)
    {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return [r * 255, g * 255, b * 255];
};

function transition(value, maximum, start_point, end_point) {
    return start_point + (end_point - start_point)*value/maximum;
}

function transition3(value, maximum,start, end) {
    r1= transition(value, maximum, start[0], end[0]);
    r2= transition(value, maximum, start[1], end[1]);
    r3= transition(value, maximum, start[2], end[2]);
    return [r1, r2, r3];
}

function roundRgb(val) {
  return Math.round(val*10000000)/10000000;
}


var boosh = require('./');

var green_hsv = rgbToHsv(0, 0, 0);
var red_hsv = rgbToHsv(255, 0, 0);
var window = boosh.createWindow();

var ctx = window.getContext('2d');
var split = require('split');
var sp = new (require('serialport').SerialPort)('/dev/tty.usbmodemfd121');

var sub = [[0,0,0,0], [0,0,0,0]];

sp.on('open' , function() {


  sp.pipe(split('\n')).on('data', function(chunk) {
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

      var hsv = transition3(part, 900, red_hsv, green_hsv);

      ctx.fillStyle = 'rgba(' + hsvToRgb(hsv).map(function(i) {
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

  window.requestAnimationFrame(function() {});

});
