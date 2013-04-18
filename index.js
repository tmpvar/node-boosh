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
  this._window = binding.createWindow();
  if (!this._window) {
    return;
  }

  this.canvas = new Canvas(640, 480);
}

util.inherits(Window, EventEmitter);

Window.prototype.flush = function() {

  // var id = this.context.getImageData(0, 0, 640, 480);
  // var b = new Buffer(id.data);
  // delete id;
//  binding.setBuffer(this.canvas);
//  delete b;
  binding.flush(this._window, this.canvas);
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


var contexts = [];

for (var i=0; i<1; i++) {
  var context = {};

  context.window = module.exports.createWindow();
  context.ctx = context.window.getContext('2d');
  context.x = 0;
  context.xv = 1;
  context.y = 0;
  context.yv = 1;

  context.color = 'rgba(' + [
    Math.floor(Math.random()*255),
    Math.floor(Math.random()*255),
    Math.floor(Math.random()*255),
    1
  ].join(',') + ')';

  contexts.push(context);
}

var timer = setTimeout(function tick() {
  contexts.forEach(function(context) {

    context.ctx.clearRect(0,0,640,480)
    context.ctx.fillStyle = context.color;
    context.x+=context.xv;
    context.y+=context.yv;

    context.ctx.save();
      context.ctx.translate(context.x, context.y);
      context.ctx.fillRect(0, 0, 100, 100);
    context.ctx.restore();

    if (context.y > 480-100 || context.y < 0) {
      context.yv = -context.yv;
    }

    if (context.x > 640-100 || context.x < 0) {
      context.xv = -context.xv;
    }

    context.window.flush();
  });
  process.nextTick(tick);
}, 1000/40);

