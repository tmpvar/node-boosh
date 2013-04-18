var util = require('util');
var EventEmitter = require('events').EventEmitter;
var binding = require('bindings')('boosh');
var Canvas = require('canvas');
var defaults = require('defaults');
var fs = require('fs');

function Window(options) {
  options = defaults({
    width: 640,
    height: 480,

    fullscreen: false,

  }, options || {})


  EventEmitter.call(this);

  if (!binding.createWindow()) {
    return;
  }

  this.canvas = new Canvas(640, 480);
}

util.inherits(Window, EventEmitter);

Window.prototype.flush = function() {

  // var id = this.context.getImageData(0, 0, 640, 480);
  // var b = new Buffer(id.data);
  // delete id;
  binding.setBuffer(this.canvas);
//  delete b;
  binding.flush();
};


function Context() {

}

Window.prototype.getContext = function(type) {
  this.context = this.canvas.getContext(type);
  return this.context;
};


/*

  options:
    width : <int>
    height: <int>
    fullscreen: <int>

*/

module.exports.createWindow = function(options) {
  var ret = new Window(options);
  if (!ret) {
    throw new Error('window could not be created');
  }
  return ret;
};

var w = module.exports.createWindow();
console.log(w);
var ctx = w.getContext('2d');
ctx.fillStyle = 'red';

var x = 0, xv = 1, y = 0, yv = 1, r = 0;
var timer = setTimeout(function tick() {
  ctx.clearRect(0,0,640,480)
  ctx.fillStyle = 'red';
  r+=.01;
  x+=xv;
  y+=yv;

  ctx.save();
    ctx.translate(x, y);
    ctx.fillRect(0, 0, 100, 100);
  ctx.restore();

  if (y > 480-100 || y < 0) {
    yv = -yv;
  }

  if (x > 640-100 || x < 0) {
    xv = -xv;
  }

  w.flush();
  process.nextTick(tick);
}, 1000/40);

