import { useEffect, useState } from 'react';

export async function readFileContent(filePath: string): Promise<string> {
  try {
    const response = await fetch(filePath);
    if (!response.ok) {
      throw new Error(`Error reading the file: ${response.statusText}`);
    }

    return await response.text();
  } catch (error) {
    console.error('Error reading the file:', error);
    throw error;
  }
}

export function useMdContent(mdPath: string): string {
  const [markdownContent, setMarkdownContent] = useState<string>('');

  useEffect(() => {
    const fetchData = async () => {
      try {
        const data = await readFileContent(mdPath);
        setMarkdownContent(data);
      } catch (error) {
        console.error('Error reading the file:', error);
        // ファイルの読み込みエラーの場合の処理
      }
    };

    fetchData().catch((error) => {
      console.error('Error in fetchData:', error);
      // エラーハンドリングが必要な場合の処理
    });
  }, [mdPath]);

  return markdownContent;
}
