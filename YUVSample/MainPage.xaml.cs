using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using System.Diagnostics;
using Windows.UI;
using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.UI.Xaml;
using YUVImageSource;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace YUVSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        CanvasDevice device;

        CanvasSwapChain swapChain;

        CanvasVirtualBitmap video;

        YUVImageSource.ImageSource imageSource;

        byte[] videoSource = null;

        #region Init Methods

        public MainPage()
        {
            this.InitializeComponent();

            videoSource = File.ReadAllBytes("sample.yuv");

            imageSource = new YUVImageSource.ImageSource();

            imageSource.createTexture(videoSource, 4000, 3000);

            imageSource.createImageSource();

            device = imageSource.getCanvasDevice();

            video = imageSource.getCanvasVirtualBitmap();
        }

        #endregion

        #region SwapChainPanel Methods

        private void swapChainPanel_Loaded(object sender, RoutedEventArgs e)
        {
            var swapChainPanel = sender as CanvasSwapChainPanel;

            this.swapChain = new CanvasSwapChain(this.device, (float)swapChainPanel.ActualWidth, (float)swapChainPanel.ActualHeight, 96);

            swapChainPanel.SwapChain = this.swapChain; 
        }

        private void swapChainPanel_CompositionScaleChanged(SwapChainPanel sender, object args)
        {
            Debug.WriteLine("swapChainPanel_CompositionScaleChanged");

            this.draw();
        }

        private void swapChainPanel_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (this.swapChain == null) { return; }

            this.swapChain.ResizeBuffers(e.NewSize);

            this.draw();
        }

        #endregion

        #region Draw Methods

        private void draw()
        {
            using (var session = swapChain.CreateDrawingSession(Colors.Black))
            {
                session.DrawImage(this.video, new Rect(0, 0, this.swapChain.Size.Width, this.swapChain.Size.Height), new Rect(0, 0, 4000, 3000));
            }

            this.swapChain.Present();
        }

        #endregion
    }
}
