var boosh = require('../');

var context = {};

context.window = boosh.createWindow({
  width : Math.floor(Math.random() * 640) + 200,
  height: Math.floor(Math.random() * 480) + 200
});

context.closeAttempts = 0;
context.window.addEventListener('close', function(ev) {
  context.closeAttempts++;
  if (context.closeAttempts <= 1) {
    ev.preventDefault();
  }
});

context.color = '#000'

context.window.addEventListener('blur', function() {
  context.color = "#ddd";
});

context.window.addEventListener('focus', function() {
  context.color = "#000";
});

context.ctx = context.window.getContext('2d');
context.x = 0;
context.xv = 1;
context.y = 0;
context.yv = 1;


context.mouse = { x: 0, y: 0, color: 'red'};
context.window.addEventListener('mousemove', function(ev) {
  var x = ev.clientX, y = ev.clientY;
  if (x + 20 > context.window.innerWidth) {
    x = context.window.innerWidth-20;
  } else if (x < 20) {
    x = 20;
  }

  if (y + 20 > context.window.outerHeight) {
    y = context.window.innerHeight-20;
  } else if (y < 20) {
    y = 20;
  }

  context.mouse.x = x;
  context.mouse.y = y;
});

context.window.addEventListener('mouseenter', function(ev) {
  context.mouse.color = "red";
});

context.window.addEventListener('mouseleave', function(ev) {
  context.mouse.color = "blue";
});

context.window.addEventListener('mousedown', function(ev) {
  var buttons = ['green', 'orange', 'purple', 'magenta', 'pink'];
  context.mouse.color = buttons[ev.button];
});

context.window.addEventListener('mouseup', function(ev) {
  context.mouse.color = "red";
});


context.text = {
  value : '',
  color : 'black'
};

context.window.addEventListener('keydown', function(ev) {

  if (ev.keyCode === 27) {
    context.window.close();
  } else {
    context.text.value += String.fromCharCode(ev.keyCode);
  }
});


context.window.requestAnimationFrame(function tick() {
  context.window.requestAnimationFrame(tick);

  context.ctx.fillStyle = '#aaa';
  context.ctx.fillRect(0,0,context.window.innerWidth,context.window.innerHeight)

  context.ctx.fillStyle = context.color;
  context.x+=context.xv;
  context.y+=context.yv;

  context.window.title = context.x + ',' + context.y;

  context.ctx.save();
    context.ctx.translate(context.x, context.y);
    context.ctx.fillRect(0, 0, 100, 100);
  context.ctx.restore();

  context.ctx.fillStyle=context.mouse.color;
  context.ctx.fillRect(context.mouse.x - 20, context.mouse.y - 20, 40, 40);

  if (context.y > context.window.innerHeight-100) {
    context.yv = -Math.abs(context.yv);
  } else if (context.y < 0) {
    context.yv = Math.abs(context.yv)
  }

  if (context.x > context.window.innerWidth-100) {
    context.xv = -Math.abs(context.xv);
  } else if (context.x < 0) {
    context.xv = Math.abs(context.xv)
  }

  context.ctx.font="20px sans-serif";
  context.ctx.fillStyle = context.text.color;
  context.ctx.fillText(context.text.value, 5, 30);
});
