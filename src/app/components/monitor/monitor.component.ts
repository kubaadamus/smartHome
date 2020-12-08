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
    });
  }
  live = false;
  liveInterval = null;
  sensorData = null;
  ngOnInit() {

    this.mainService.getSensorData();
  }
  toggleLive() {
    this.live = !this.live;
    if (this.live === true) {
      this.liveInterval = setInterval(() => {
        this.mainService.getSensorData();
      }, 1000);
    }
    else {
      clearInterval(this.liveInterval);
    }
  }
}