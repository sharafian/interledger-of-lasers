const ILP = require('ilp')
const PluginBtp = require('ilp-plugin-payment-channel-framework')

async function trySendingPayment (server) {
  console.log('creating plugin for "%s"', server)
  const plugin = new PluginBtp({ server, insecure: true })
  await plugin.connect()  
  console.log('connected plugin')

  const payment = await ILP.SPSP.quote(plugin, {
    receiver: 'http://localhost:2010/api/spsp/admin',
    destinationAmount: '0.001'
  })

  console.log('quoted payment:', payment)
  payment.memo = { message: 'hello' }

  await ILP.SPSP.sendPayment(plugin, payment)
  console.log('receiver claimed funds!')
}

trySendingPayment('btp+ws://:verysecretsecret@localhost:3000')
