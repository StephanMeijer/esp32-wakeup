import React, { Component } from 'react';
import './App.css';
import { SketchPicker } from 'react-color';
import axios from 'axios';
import querystring from 'querystring';

class App extends Component {
  state = {
    color: '#fff',
  };

  handleChangeComplete = (color) => {
    axios.post('/lights/1/mode', querystring.stringify({
      a: parseInt(color.rgb.a * 255),
      r: color.rgb.r,
      g: color.rgb.g,
      b: color.rgb.b
    })).then(function (response) {
      console.log(response);
    })
    .catch(function (error) {
      console.log(error);
    });
    this.setState({
      color: color.rgb
    });
  };

  render() {
    return (
      <div className="App">
        <nav className="navbar navbar-expand-lg navbar-light bg-light">
          <a className="navbar-brand" href={'/'}>MENU</a>
          <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
            <span className="navbar-toggler-icon"></span>
          </button>

          <div className="collapse navbar-collapse" id="navbarSupportedContent">
            <ul className="navbar-nav mr-auto">
              <li className="nav-item">
                <a className="nav-link" href={'/'}>Moodlight</a>
              </li>
              <li className="nav-item">
                <a className="nav-link" href={'/'}>Wekkers</a>
              </li>
            </ul>
          </div>
        </nav>
        <div className="container">
          <SketchPicker
            color={ this.state.color }
            onChangeComplete={ this.handleChangeComplete } />
        </div>
      </div>
    );
  }
}

export default App;
