import "./App.css";
import React from "react";
import { useState, useEffect, useRef } from "react";
import { w3cwebsocket as W3CWebSocket } from "websocket";

export default function App() {
  const websocket = useRef(null);
  const [relayState, setRelayState] = useState([0, 0, 0, 0, 0, 0, 0]);

  const toggleButtonState = index => {
    const states = [...relayState];
    states[index] = states[index] ? 0 : 1;
    setRelayState(states);
    const data = {
      type: "control",
      data: relayState,
    };
    websocket.current.send(JSON.stringify(data));
  };

  useEffect(() => {
    websocket.current = new W3CWebSocket("ws://192.168.2.1/ws");
    websocket.current.onmessage = message => {
      const dataFromServer = JSON.parse(message.data);
      console.log(dataFromServer);
      if (dataFromServer.type === "ack") {
        // setRelayState(dataFromServer.data);
      }
    };
    return () => websocket.current.close();
  }, []);

  return (
    <main className="w-screen h-screen">
      <section className="p-5 pt-10 h-48">
        <div className="h-full bg-blue-400 rounded-xl p-4">
          <h1>Lorem ipsum, dolor sit amet</h1>
        </div>
      </section>
      <section className="px-5 pt-10 grid grid-cols-2 gap-4">
        {Array.from({ length: 6 }, (_, index) => {
          return (
            <React.Fragment key={index}>
              <button
                id={index}
                className={`w-full h-24 border rounded-xl shadow-lg ${relayState[index] ? "shadow-green-500" : "shadow-red-500"}`}
                value={relayState[index]}
                onClick={() => toggleButtonState(index)}
                style={{ WebkitTapHighlightColor: "transparent" }}>
                <p>{relayState[index]}</p>
              </button>
            </React.Fragment>
          );
        })}
      </section>
    </main>
  );
}
