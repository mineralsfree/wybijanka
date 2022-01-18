const [node, file, ip, port] = process.argv
const net = require('net');
const ws1 = require('ws');

function chunk(buf, maxBytes) {
    const result = [];
    while (buf.length) {
        result.push(buf.slice(0, maxBytes).toString());
        buf = buf.slice(maxBytes);
    }
    return result.map(el=>el.slice(0, el.indexOf(String.fromCharCode(0))))
}

const wss = new ws1.WebSocketServer({port: 8080});
const write = (soc, msg, id) => {
    const buffer = msg;
    console.log(msg.toString());
    console.log(id);
    console.log(Buffer.concat([buffer]));
    soc.write(Buffer.concat([buffer], 100));
}
const users = {}
wss.on('connection', function connection(ws, request, client) {
    const socket = new net.Socket();
    socket.connect(port, ip);
    ws.id = request.headers['sec-websocket-key'];
    users[request.headers['sec-websocket-key']] = {socket};
    users[request.headers['sec-websocket-key']].data = '';
    socket.on('data', data => {
        users[request.headers['sec-websocket-key']].data += data.toString();
        console.log(users[request.headers['sec-websocket-key']].data);
        if (users[request.headers['sec-websocket-key']].data.length % 100 === 0) {
            const messages = chunk(users[request.headers['sec-websocket-key']].data, 100);
            console.log(messages)
            messages.forEach(message => {
                ws.send(message);
            })
            users[request.headers['sec-websocket-key']].data = '';
        }

    })
    ws.on('message', function message(data) {
        write(users[this.id].socket, data, this.id);
    })
})


