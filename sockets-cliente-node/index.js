const net = require('net');
const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

const PORT = 8080;
const HOST = '127.0.0.1';

const client = new net.Socket();

client.connect(PORT, HOST, () => {
    console.log('Conectado al servidor en ' + HOST + ':' + PORT);
    rl.question('Menú:\n1. Generar Nombre de Usuario\n2. Generar Contraseña\nIntroduce tu opción: ', (option) => {
        if (option === '1') {
            rl.question('Introduce la longitud del nombre de usuario (5-15): ', (length) => {
                client.write('username');
                client.write(length);
            });
        } else if (option === '2') {
            rl.question('Introduce la longitud de la contraseña (8-50): ', (length) => {
                client.write('password');
                client.write(length);
            });
        }
    });
});

client.on('data', (data) => {
    console.log('Respuesta del servidor: ' + data.toString());
    rl.close();
    client.destroy();
});

client.on('close', () => {
    console.log('Conexión cerrada');
});

client.on('error', (err) => {
    console.error('Error en la conexión: ' + err.message);
});
