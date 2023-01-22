import React, { useState, useEffect } from 'react';
import * as Realm from "realm-web";
import Steps from "./components/Steps.js";

// Add your App ID
const app = new Realm.App({ id: "data-wpsdz" });

// Create a component that lets an anonymous user log in
function Login({ setUser }) {
  const loginAnonymous = async () => {
    const user = await app.logIn(Realm.Credentials.anonymous());
    setUser(user);
  };
  return <button onClick={loginAnonymous}>Log In</button>;
}

const App = () => {
  // Keep the logged in Realm user in local state. This lets the app re-render
  // whenever the current user changes (e.g. logs in or logs out).
  const [user, setUser] = React.useState(app.currentUser);

  // If a user is logged in, show their details.
  // Otherwise, show the login screen.
  return (
    <div className="App">
      <div className="App-header">
        {user ? <Steps user={user} /> : <Login setUser={setUser} />}
      </div>
    </div>
  );
};
export default App;
