import { Button } from '@mui/material'
import { useContext } from 'react';
import { UserContext } from '../contexts/user.context';
import { Steps } from './components/Steps.js';
import { Reuse } from './components/Reuse.js';
import './Home.page.css'
 
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
    <div className='body'>
      <div className='navbar'>
          <h2 className='welcome'>
            {user._profile.data.email}
            <Button className='logout'  onClick={logOut}>Logout</Button>
          </h2>
      </div>
      <table className='data'>
        <tr>
          <th className='row'>
          <Steps user={user}/>
          </th>
          <th className='row'>
          <Reuse user={user}/>
          </th>
        </tr>
      </table>
    </div>
 )
}