const mqtt = require('mqtt')
const Express = require('express')
const app = Express()

const client = mqtt.connect('mqtt://192.168.3.7', { connectTimeout: 100, reconnectPeriod: 100 })

app.set('view engine', 'ejs')
app.set('views', './views')
app.use(Express.static(__dirname + '/views'))
app.get('/', function (req, res) {
    res.render("index")
})


app.get('/:element', function (req, res) {
  var element = req.params.element;
  console.log(element)
  if(element != "favicon.ico")
    client.publish("Color", element)
  res.send(element)
})

app.listen(3000)