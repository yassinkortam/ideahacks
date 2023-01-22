import React from "react";
import { createRoot } from 'react-dom/client';
import App from "./App";
import { BrowserRouter } from "react-router-dom/dist";
import "./index.css"

const domNode = document.getElementById('root');
const root = createRoot(domNode);
root.render(
  <React.StrictMode>
    <BrowserRouter>
      <App />
    </BrowserRouter>
  </React.StrictMode>,
);