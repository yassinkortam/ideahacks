import React, { useState, useEffect } from 'react';
import { Sankey, ResponsiveContainer } from 'recharts';
import "./Reuse.css";

// Create a component that displays the given user's details
export function Reuse({ user }){
    const [result, setResult] = useState(
      {
       "reuse":0
      }
    );
  
    useEffect(() => {
      async function getData(){
        const result = await user.functions.getStats(user._profile.data.email);
        setResult(result)
      }
      getData();
    });

    const data = {
        "nodes" : [
            {
                "name" : "origin"
            },
            {
                "name" : "reuse"
            },
            {
                "name" : "waste"
            }
        ],
        "links" : [
            {
                "source" : 0,
                "target" : 2,
                "value" : result["reuse"]/(result["reuse"] + 20)
            },
            {
                "source" : 0,
                "target" : 1,
                "value" : 20/(result["reuse"] + 20)
            }
        ]
    }

    return (
      <div className="reuse-container">
        <h1 className="reuse"> {result["reuse"]} Reuse Events </h1>
          <ResponsiveContainer width="100%" height="100%">
            <Sankey
                width={960}
                height={500}
                data={data}
                node={{stroke: '#77c878', strokeWidth: 2}}
                nodePadding={50}
                margin={{
                    left: 200,
                    right: 200,
                    top: 100,
                    bottom: 100,
                }}
                link={{ stroke: '#77c878' }}
            >
            </Sankey>
        </ResponsiveContainer>
      </div>
    );
}
