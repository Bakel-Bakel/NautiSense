import React, { useRef } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';

function Boat({ roll, pitch, yaw }) {
  const ref = useRef();
  const toRad = (deg) => (deg * Math.PI) / 180;

  useFrame(() => {
    if (ref.current) {
      ref.current.rotation.x = toRad(pitch);
      ref.current.rotation.y = toRad(yaw);
      ref.current.rotation.z = toRad(roll);
    }
  });

  return (
    <mesh ref={ref}>
      <boxGeometry args={[2, 0.5, 1]} />
      <meshStandardMaterial color="royalblue" />
    </mesh>
  );
}

const Boat3D = ({ roll, pitch, yaw }) => (
  <div style={{ width: '100%', height: '400px' }}>
    <Canvas camera={{ position: [0, 3, 5], fov: 60 }}>
      <ambientLight />
      <directionalLight position={[5, 5, 5]} />
      <Boat roll={roll} pitch={pitch} yaw={yaw} />
    </Canvas>
  </div>
);

export default Boat3D;
