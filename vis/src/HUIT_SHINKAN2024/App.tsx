import { FC } from 'react';
import { Routes, Route } from 'react-router-dom';
import Home from 'HUIT_SHINKAN2024/routes/Home/Home';
import NotFound from 'HUIT_SHINKAN2024/routes/NotFound/NotFound';
import { VisualizerInfoProvider } from 'HUIT_SHINKAN2024/components/BaseVisualizer/context/VisualizerInfoProvider';
import './App.css';

const App: FC = () => {
  return (
    <div>
      <Routes>
        <Route
          path="/HUIT_SHINKAN2024/"
          element={
            <VisualizerInfoProvider>
              <Home />
            </VisualizerInfoProvider>
          }
        />
        <Route path="*" element={<NotFound />} />
      </Routes>
    </div>
  );
};

export default App;
