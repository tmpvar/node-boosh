var util = require('util');
var EventEmitter = require('events').EventEmitter;
var binding = require('bindings')('boosh');
var Canvas = require('canvas');
var defaults = require('defaults');
var fs = require('fs');
var NativeWindow = binding.Window;

function Window(options) {
  console.log(options);
  options = defaults(options || {}, {
    width: 640,
    height: 480,
    title: 'boosh!',
    fullscreen: false,
  })
options ={
    width: 640,
    height: 480,
    title: 'boosh!',
    fullscreen: false,
  };

  EventEmitter.call(this);
  this._window = new NativeWindow(options.width, options.height, options.title);

  if (!this._window) {
    return;
  }

  this._window.eventHandler(function(raw) {


  }.bind(this));


  this.width = options.width;
  this.height = options.height;

  this.canvas = new Canvas(options.width, options.height);
  console.log(this.canvas.width, this.canvas.height);
}

//util.inherits(Window, EventEmitter);

Window.prototype.flush = function() {
  this._window.flush();
};

Window.prototype.getContext = function(type) {
  if (this.context) {
    throw new Error('context already created');
  }

  if (type === '2d') {
    this.context = this.canvas.getContext(type);
    this._window.get2dContext(this.canvas);
    return this.context;
  }
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

  context.window = module.exports.createWindow({
    width : Math.floor(Math.random() * 640),
    height: Math.floor(Math.random() * 480)
  });

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

    context.ctx.clearRect(0,0,context.window.width,context.window.height)
    context.ctx.fillStyle = 'red';//context.color;
    context.x+=context.xv;
    context.y+=context.yv;

    context.ctx.save();
      context.ctx.translate(context.x, context.y);
      context.ctx.fillRect(0, 0, 100, 100);
    context.ctx.restore();

    if (context.y > context.window.height-100 || context.y < 0) {
      context.yv = -context.yv;
    }

    if (context.x > context.window.width-100 || context.x < 0) {
      context.xv = -context.xv;
    }

    context.window.flush();
  });
  process.nextTick(tick);
}, 1000/40);
setTimeout(function() {
  contexts[0].window._window.resizeTo(100, 100);
}, 2000);

