import { Injectable } from '@angular/core';
import { Subject } from 'rxjs/Subject';

@Injectable({
  providedIn: 'root'
})
export class MainService {
  serviceResponse = new Subject(); // Ogólny vent odpalany jako odpowiedź, że coś przyszło z API (do chowania loadingu)
  constructor() { }
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
}
