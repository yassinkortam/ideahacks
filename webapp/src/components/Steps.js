import React, { useState, useEffect } from 'react';
import { RadialBarChart, RadialBar, Legend, ResponsiveContainer } from 'recharts';
import "./Steps.css"

// Create a component that displays the given user's details
export default function Steps({ user }){
    const [result, setResult] = useState(null);
  
    useEffect(() => {
      async function getData(){
        const result = await user.functions.getStats("63cc3b7d0cdd2b2c8dee4435");
        console.log(user.id)
        setResult(result)
      }
      getData();
    });

    console.log(result)

    const style = {
      top: '50%',
      right: 0,
      transform: 'translate(0, -50%)',
      lineHeight: '24px',
    };

    const data = [{
        name: 'score', 
        value: 1000, 
        fill: '#FFD700'
    },];

    return (
      <div classname="step-counter">
        <h1 classname="test">test</h1>
        <ResponsiveContainer width="100%" height="100%">
          <RadialBarChart cx="50%" cy="50%" innerRadius="10%" outerRadius="80%" barSize={10} data={data}>
            <RadialBar
              minAngle={15}
              background
              clockWise
              dataKey="value"
            />
            <Legend iconSize={10} layout="vertical" verticalAlign="middle" wrapperStyle={style} />
          </RadialBarChart>
        </ResponsiveContainer>
      </div>
    );
}
