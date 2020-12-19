import { Injectable } from '@angular/core';
import { Subject } from 'rxjs/Subject';

@Injectable({
  providedIn: 'root'
})
export class MainService {
  serviceResponse = new Subject(); // Ogólny vent odpalany jako odpowiedź, że coś przyszło z API (do chowania loadingu)
  websocket;
  gateway;

  constructor() {
    //  this.initWebSocket();
  }
  createPromise(request, apiPath = null) {
    if (apiPath == null) {
      apiPath = 'http://www.fitcalc.cba.pl/backend.php';
    }
    return new Promise((resolve, reject) => {
      const xhttp = new XMLHttpRequest();
      let formData = new FormData();
      formData.append('object', encodeURIComponent(JSON.stringify(request)));
      xhttp.onload = (x) => {
        let resultObject;
        try {
          resultObject = JSON.parse(xhttp.responseText);
        } catch (error) {
          resultObject = xhttp.responseText;
        }
        console.log("Odebralem:");
        console.log(resultObject);
        if (resultObject !== null) {
          resolve(resultObject);
        } else { reject('Failed'); }

      }
      xhttp.open('post', apiPath);
      xhttp.send(formData);
    });
  }
  getServiceResponse() { // EVENT - zwraca dane z promisa albo z czegokolwiek
    return this.serviceResponse.asObservable();
  }
  getSensorData(request = null) { // Pobiera wszystkie ordery wraz z elementami (do Twoich Zamówień)
    console.log('Odpalam pobieranie danych sensora');
    request = { action: 'getSensorData' };
    const c = this.createPromise(request);
    c.then((onmessage) => {
      this.serviceResponse.next({ mode: 'sensorDataReceived', data: onmessage });
    }).catch((onmessage) => {
    });
  }
  updateInput(request = null, inputs) { // Pobiera wszystkie ordery wraz z elementami (do Twoich Zamówień)
    console.log('Odpalam pobieranie danych sensora');
    request = { action: 'updateInput', inputs: inputs };
    const c = this.createPromise(request);
    c.then((onmessage) => {
      this.serviceResponse.next({ mode: 'inputUpdated', data: onmessage });
    }).catch((onmessage) => {
    });
  }
  getInput(request = null) { // Pobiera wszystkie ordery wraz z elementami (do Twoich Zamówień)
    console.log('Odpalam pobieranie danych z tabeli input');
    request = { action: 'getInput' };
    const c = this.createPromise(request);
    c.then((onmessage) => {
      this.serviceResponse.next({ mode: 'inputReceived', data: onmessage });
    }).catch((onmessage) => {
    });
  }
  getRequests(request = null) { // Pobiera wszystkie ordery wraz z elementami (do Twoich Zamówień)
    console.log('Odpalam pobieranie requestow');
    request = { action: 'getRequests' };
    const c = this.createPromise(request);
    c.then((onmessage: any) => {
      this.serviceResponse.next({ mode: 'requestsReceived', requests: onmessage.requests});
    }).catch((onmessage) => {
    });
  }
  initWebSocket() {
    this.gateway = `ws://89.229.94.96:80/ws`;  // /ws    // 	188.146.131.112 - publi ip  // 100.107.251.181 - router
    this.websocket = new WebSocket(this.gateway);
    console.log("gateway: " + this.gateway);
    console.log('Trying to open a WebSocket connection...');
    this.websocket.onopen = this.onOpen;
    this.websocket.onclose = this.onClose;
    this.websocket.onmessage = this.onMessage; // <-- add this line
  }
  onOpen(event) {
    console.log('Connection opened');
  }
  onClose(event) {
    console.log('Connection closed');
    setTimeout(this.initWebSocket, 2000);
  }
  onMessage(event) {
    console.log("Dostalem z node!");
    console.log(event);
  }
  sendData(message) {
    this.websocket.send(message);
  }
  createRequest(request) {
    console.log('Daję nowy request dla systemu arduino');
    request = { action: 'createRequest', request: request };
    const c = this.createPromise(request);
    c.then((onmessage) => {
      this.serviceResponse.next({ mode: 'requestCreated', onmessage });
    }).catch((onmessage) => {
    });
  }
}