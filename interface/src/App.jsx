import "./App.css";
import React from "react";
import { useState, useEffect, useRef } from "react";
import { w3cwebsocket as W3CWebSocket } from "websocket";
import logo from "./assets/Logo.png";

const buttonName = ["Init", "Start Program", "Freedrive", "Relay 3", "Relay 4", "Relay 5", "Relay 6"];

export default function App() {
  const websocket = useRef(null);
  const [buttonState, setButtonState] = useState([0, 0, 0, 0, 0, 0]);

  const toggleButtonState = index => {
    const states = [...buttonState];
    states[index] = states[index] ? 0 : 1;
    setButtonState(states);
    const data = {
      type: "control",
      data: states,
    };
    websocket.current.send(JSON.stringify(data));
  };

  useEffect(() => {
    websocket.current = new W3CWebSocket("ws://192.168.2.1/ws");
    websocket.current.onmessage = message => {
      const dataFromServer = JSON.parse(message.data);
      console.log(dataFromServer);
      if (dataFromServer.type === "ack") {
        // setButtonState(dataFromServer.data);
      }
    };
    return () => websocket.current.close();
  }, []);

  return (
    <main className="w-screen h-screen">
      <section className="p-5 pb-0">
        <div className="flex">
          <div>
            <img src={logo} alt="" className="w-16" />
          </div>
          <h1 className="ml-3 font-semibold w-52">ROBOTICS AND INTELLIGENT SYSTEM LABORATORY</h1>
        </div>
        <div className="mt-6 px-5">
          <h1 className="text-center font-bold text-2xl">UNIVERSAL ROBOT CONTROLLER</h1>
        </div>
      </section>
      <section className="mt-8 px-5 grid grid-cols-2 gap-4 gap-x-2">
        {Array.from({ length: 6 }, (_, index) => {
          return (
            <React.Fragment key={index}>
              <button
                id={index}
                className={`w-full h-24 border-[#393737] border-4 rounded-md ${buttonState[index] ? "active" : "inactive"}`}
                value={buttonState[index]}
                onClick={() => toggleButtonState(index)}
                style={{ WebkitTapHighlightColor: "transparent" }}>
                <p>{`${buttonName[index]}`}</p>
              </button>
            </React.Fragment>
          );
        })}
      </section>
    </main>
  );
}
