import React, { useState, useEffect } from 'react';
import { RadialBarChart, RadialBar, ResponsiveContainer } from 'recharts';
import "./Steps.css";

// Create a component that displays the given user's details
export default function Steps({ user }){
    const [result, setResult] = useState(
      {
       "steps":0
      }
    );
  
    useEffect(() => {
      async function getData(){
        const result = await user.functions.getStats("63cc3b7d0cdd2b2c8dee4435");
        console.log(user.id)
        setResult(result)
      }
      getData();
    });

    console.log(result)

    const data = [
      {
        name: 'Goal', 
        value: 10000, 
        fill: '#FAFAFA'
      },
      {
        name: 'Steps', 
        value: result["steps"], 
        fill: '#000080'
      }
    ];

    return (
      <div className="step-counter">
        <h1 className="steps"> {result["steps"]} steps </h1>
          <ResponsiveContainer width="100%" height="100%">
            <RadialBarChart cx="50%" cy="50%" innerRadius="50%" outerRadius="50%" barSize={5} data={data}>
              <RadialBar minAngle={15} background clockWise dataKey="value" />
            </RadialBarChart>
          </ResponsiveContainer>
        r</div>
    );
}
