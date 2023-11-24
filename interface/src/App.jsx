import './App.css';
import React from 'react';
import { useState } from 'react';

export default function App() {
  const [buttonState, setButtonState] = useState([false, false, false, false, false, false]);

  const toggleButtonState = (index) => {
    const states = [...buttonState];
    states[index] = !states[index];
    setButtonState(states);
  }
  return (
    <main className='w-screen h-screen'>
      <section className='p-5 pt-10 h-48'>
        <div className='h-full bg-blue-400 rounded-xl p-4'>
          <h1>Lorem ipsum, dolor sit amet</h1>
        </div>
      </section>
      <section className='px-5 pt-10 grid grid-cols-2 gap-4'>
        {
          Array.from({ length: 6 }, (_, index) => {
            return (
              <React.Fragment key={index}>
                <button id={index}
                  className={`w-full h-24 border rounded-xl shadow-lg ${(buttonState[index]) ? "shadow-green-500" : "shadow-red-500"}`}
                  value={buttonState[index]}
                  onClick={() => toggleButtonState(index)}
                  style={
                    {WebkitTapHighlightColor: "transparent"}
                  }
                >
                  <p>{buttonState[index]}</p>
                </button>
              </React.Fragment>
            )
          })
        }
      </section>
    </main>
  )
}