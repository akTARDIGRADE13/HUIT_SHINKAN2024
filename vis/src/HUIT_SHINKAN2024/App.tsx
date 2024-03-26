import { FC } from 'react';
import { Routes, Route } from 'react-router-dom';
import { Helmet } from 'react-helmet';
import Home from 'HUIT_SHINKAN2024/routes/Home/Home';
import NotFound from 'HUIT_SHINKAN2024/routes/NotFound/NotFound';
import { VisualizerInfoProvider } from 'HUIT_SHINKAN2024/components/BaseVisualizer/context/VisualizerInfoProvider';
import './App.css';

const App: FC = () => {
  return (
    <>
      <Helmet>
        <title>HUIT新歓2024_vis</title>
        <meta name="description" content="LT用のVisualizer" />
        <meta property="og:title" content="Visualizer" />
        <meta property="og:description" content="スライドパズルのVisualizer" />
        <meta
          property="og:image"
          content="https://aktardigrade13.github.io/HUIT_SHINKAN2024/banner-meta.jpg"
        />
        <meta property="og:image:width" content="1200" />
        <meta property="og:image:height" content="670" />
        <meta property="og:image:type" content="image/jpg" />
        <meta property="og:image:alt" content="バナー" />
        <meta name="twitter:card" content="summary_large_image" />
        <meta name="twitter:site" content="@akTARDIGRADE13"></meta>
        <meta
          property="og:url"
          content="https://aktardigrade13.github.io/HUIT_SHINKAN2024/"
        />
        <meta property="og:type" content="website" />
        <meta property="og:locale" content="ja_JP" />
      </Helmet>
      <Routes>
        <Route
          path="/"
          element={
            <VisualizerInfoProvider>
              <Home />
            </VisualizerInfoProvider>
          }
          exact
        />
        <Route path="*" element={<NotFound />} />
      </Routes>
    </>
  );
};

export default App;
