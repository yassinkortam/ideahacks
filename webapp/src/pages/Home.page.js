import { Button } from '@mui/material'
import { useContext } from 'react';
import { UserContext } from '../contexts/user.context';
import { Steps } from './components/Steps.js';
 
export default function Home() {
 const { logOutUser } = useContext(UserContext);
 const { user } = useContext(UserContext);
 
 // This function is called when the user clicks the "Logout" button.
 const logOut = async () => {
   try {
     // Calling the logOutUser function from the user context.
     const loggedOut = await logOutUser();
     // Now we will refresh the page, and the user will be logged out and
     // redirected to the login page because of the <PrivateRoute /> component.
     if (loggedOut) {
       window.location.reload(true);
     }
   } catch (error) {
     alert(error)
   }
 }
 
return (
    <div>
        <h1>Hi,  {user._profile.data.email}</h1>
        <Steps user={user}/>
        <Button variant="contained" onClick={console.log(user)}>Logout</Button>
    </div>
 )
}