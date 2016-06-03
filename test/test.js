var binding = require('bindings')('boosh');

var window = new binding.Window(500, 500, 'hello')
var resizing =
window.eventHandler(function(e) {
  if (e.type === 'close' || e.type === 'keyup' && e.keyCode === 27) {
    clearInterval(timer)
    return window.close();
  }

  if (e.type === 'resize') {
    render()
  }

  console.log(e)
})

var TAU = Math.PI*2

var rect = window.getRect()



var ctx = new binding.Nanovg()
// var gl = require('gl')(500, 500, { preserveDrawingBuffer: false })

ctx.beginFrame()
  ctx.save()
    var mat = new Float32Array(6)
    ctx.getCurrentTransform(mat)
    console.log(mat.toString());

    ctx.translate(100, 100)
    ctx.getCurrentTransform(mat)
    console.log(mat.toString())

    mat[5] = 123
    ctx.resetTransform()


    ctx.getCurrentTransform(mat)
    console.log(mat.toString())

  ctx.restore()
ctx.cancelFrame()

var x = 0
var r = 0

function render() {
  window.beginFrame()
  rect = window.getRect()
  ctx.beginFrame(rect.width, rect.height, rect.fbWidth / rect.width)

    ctx.save()
      ctx.beginPath()
        ctx.moveTo(200, 100)
        ctx.arc(100, 100, 100, 0, Math.PI*2, false)

        ctx.moveTo(200, 100)
        ctx.rect(200, 100, 50, 100)

        ctx.translate(-70, 150)
        ctx.moveTo(112, 229)
        ctx.arcTo(300, 25, 500, 225, 75)
        ctx.lineTo(500, 225)

        ctx.translate(100, 0)
        ctx.rotate(Date.now())
        ctx.rect(0, 0, 100, 50)

        ctx.roundedRect(50, 100, 50, 50, 10)
        ctx.ellipse(50, 200, 50, 10)
      ctx.setFillColor(255, 0, 0, 255)
      ctx.fill()
      ctx.setStrokeColor(255, 255, 255, 255)
      ctx.strokeWidth(5)
      ctx.stroke()
    ctx.restore()

    ctx.save()
      ctx.beginPath()
        ctx.translate(50, 50)
        // TODO: expose a version of this function that pre-clamps
        ctx.rotate(((Date.now()/10000/TAU) % 1) * TAU)
        ctx.scale(20, 200)
        ctx.rect(0, 0, 1, 1)

        ctx.setFillColor(0, 255, 0, 255)
        ctx.fill()
    ctx.restore()

    ctx.save()
      ctx.translate(250, 250)
      ctx.scale(2, 2)
      ctx.strokeWidth(1)
      ctx.setFillColor(0, 255, 0, 200)
      ctx.setStrokeColor(127, 255, 0, 200)

      ctx.beginPath()

      for (var i=0; i<1000; i++) {
        ctx.circle(
          Math.sin(Date.now()/1000) * Math.sin(i * 2) * 50 * Math.cos(i*2),
          Math.cos(i * 2) * 50,
          1
        )

        ctx.circle(
          Math.cos(3/i) * Math.sin(i) * 50,
          Math.sin(1/i * Date.now()) * Math.cos(i) * 50,
          1
        )

        ctx.circle(Math.sin(i) * 50 * Math.cos(Date.now()/5000000), Math.cos(Math.sin(Date.now()/1000000) * i) * 50, 3)
      }
      ctx.fill()
      ctx.stroke()

      ctx.setFillColor(255, 255, 0, 200)
      ctx.setStrokeColor(255, 70, 127, 200)
      ctx.beginPath()
      for (var i=0; i<1000; i++) {

        ctx.circle(
          Math.cos(i*2) * 50 * Math.sin(i) + i/Math.sin(i),
          Math.sin(Date.now()/1000 + i) * 50,
          2
        )

       // ctx.circle(
       //    Math.cos(i*2) * 50 * Math.sin(i) - i/Math.sin(Date.now()/1000),
       //    Math.sin(Date.now()/1000 + i) * 50 * Math.cos(i * 2),
       //    2
       //  )

      }

      ctx.fill()
      ctx.stroke()

    ctx.restore()

  ctx.endFrame()


  window.endFrame()
}


var timer = setInterval(render, 0)
