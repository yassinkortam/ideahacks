import React, { useState, useEffect } from 'react';
import { RadialBarChart, RadialBar, ResponsiveContainer } from 'recharts';

// Create a component that displays the given user's details
export default function Steps({ user }){
    const [result, setResult] = useState(null);
  
    useEffect(() => {
      async function getData(){
        const result = await user.functions.getStats(user.id);
        setResult(result)
      }
      getData();
    });

    const chartData = [{
        name: 'score', 
        value: result["steps"], 
        fill: '#FFD700'
    },];

    const getMaxAngle = (data) => {
        return 270 * Math.min(data[0].value || 0, 1000) / 1000;
    };
  
    return (
            <RadialBarChart 
                height = {500}
                width = {500}
                outerRadius={500} 
                barSize={400} 
                data={chartData}>
                <RadialBar 
                    startAngle={-1 * 135} 
                    endAngle={-1 * 405} 
                    maxAngle={getMaxAngle(chartData)} 
                    background 
                    clockWise={true} 
                    dataKey='value'/>
            </RadialBarChart>

    );
}
