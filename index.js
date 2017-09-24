const ILP = require('ilp')
const PluginBtp = require('ilp-plugin-payment-channel-framework')
const SerialPort = require('serialport')
const PORT_NAME = process.env.PORT_NAME || '/dev/cu.usbmodem1421'
const port = new SerialPort(PORT_NAME, {
  baudRate: 9600
})

process.on('unhandledRejection', (e) => console.error(e))

port.on('open', () => {
  console.log('opened serial port', PORT_NAME)
})

let buf = Buffer.from('')
port.on('data', data => {
  if (data.toString() === '\n') {
    console.log('got full string: "%s"', buf.toString())
    trySendingPayment(buf.toString())
    buf = Buffer.from('')
  } else {
    buf = Buffer.concat([ buf, data ])
  }
})

async function trySendingPayment (server) {
  console.log('creating plugin for "%s"', server)
  const plugin = new PluginBtp({ server, insecure: true })
  await plugin.connect()  
  console.log('connected plugin')

  const payment = await ILP.SPSP.quote(plugin, {
    receiver: 'ben@localhost:3001',
    destinationAmount: '1000'
  })

  console.log('quoted payment:', payment)
  payment.memo = { message: 'hello' }

  await ILP.SPSP.sendPayment(plugin, payment)
  console.log('receiver claimed funds!')
}