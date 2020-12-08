import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { MenuComponent } from './components/menu/menu.component';
import { PanelTopComponent } from './components/panel-top/panel-top.component';
import { MonitorComponent } from './components/monitor/monitor.component';
import { LogsComponent } from './components/logs/logs.component';

const routes: Routes = [
  { path: '', component: MonitorComponent }
];

@NgModule({
  declarations: [
    AppComponent,
    MenuComponent,
    PanelTopComponent,
    MonitorComponent,
    LogsComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    RouterModule.forRoot(routes)
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
