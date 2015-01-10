net = require('net')

port = 3030

server = net.createServer (client) ->
  console.log "server connected from #{client.remoteAddress}!"

  client.on 'data', (data) ->
    console.log "client sent: #{data.toString()}"

  client.on 'end', ->
    console.log 'server disconnected!'

server.listen port

console.log "listening on port #{port}"
