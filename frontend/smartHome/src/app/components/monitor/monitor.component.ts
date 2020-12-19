import { MainService } from './../../services/main.service';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-monitor',
  templateUrl: './monitor.component.html',
  styleUrls: ['./monitor.component.scss']
})
export class MonitorComponent implements OnInit {
  subscription = null;
  constructor(public mainService: MainService) {
    this.subscription = this.mainService.getServiceResponse().subscribe((onmessage: any) => {
      if (onmessage.mode == 'sensorDataReceived') {
        this.sensorData = onmessage.data.sensorData;
      }
      if (onmessage.mode == 'inputReceived') {
        this.inputs = onmessage.data.inputs;
      }
      if (onmessage.mode == 'requestsReceived') {
        console.log("mam te requesty");
        console.log(onmessage);
        this.requests = onmessage.requests;
      }
      if (onmessage.mode == 'requestCreated') {
        this.mainService.getRequests();
      }
    });
  }
  live = false;
  liveInterval = null;
  sensorData = null;
  requests = null;
  inputs = null;

  ngOnInit() {
    this.mainService.getRequests()
    this.mainService.getSensorData();
    this.mainService.getInput();
;
    this.toggleLive();
  }
  toggleLive() {
    this.live = !this.live;
    if (this.live === true) {
      this.liveInterval = setInterval(() => {
        this.mainService.getRequests();
        this.mainService.getSensorData();
        this.mainService.getInput();
      }, 2000);
    }
    else {
      clearInterval(this.liveInterval);
    }
  }

}
